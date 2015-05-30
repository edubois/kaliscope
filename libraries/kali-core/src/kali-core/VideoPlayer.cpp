#include "VideoPlayer.hpp"

#include <tuttle/common/utils/global.hpp>
#include <Sequence.hpp>

#include <vector>
#include <memory>

namespace kaliscope
{

VideoPlayer::VideoPlayer( const std::shared_ptr<tuttle::host::Graph> & graph )
: _graph( graph )
{
    initialize();
}

VideoPlayer::~VideoPlayer()
{
    stop();
    terminate();
}

/**
 * @brief reset processing graph to default
 */
void VideoPlayer::resetProcessingGraphToDefault()
{
    _graph.reset();
    initialize();
}

/**
 * @brief set processing graph
 * @param graph new processing graph
 * @return previous processing graph
 */
std::shared_ptr<tuttle::host::Graph> VideoPlayer::setProcessingGraph( const std::shared_ptr<tuttle::host::Graph> & graph )
{
    stop();
    std::shared_ptr<tuttle::host::Graph> previousGraph = _graph;
    _graph = graph;
    initialize();
    return previousGraph;
}

/**
 * @brief initialize all
 */
void VideoPlayer::initialize()
{
    using namespace tuttle::host;
    try
    {
        std::unique_lock<std::mutex> lock( _mutexPlayer );
        if ( !_graph )
        {
            _graph.reset( new tuttle::host::Graph() );
            _nodeRead = &_graph->createNode( "tuttle.avreader" );
            if ( _nodeRead )
            {
                // We want an rgb output
                _nodeRead->getParam( "channel" ).setValue( 2 );
                _nodeRead->getParam( "bitDepth" ).setValue( 1 );

                buildGraph();
            }
        }
        else
        {
            std::vector<Graph::Node*> nodes = _graph->getNodes();
            for( Graph::Node* node: nodes )
            {
                if ( _graph->getNbInputConnections( *node ) == 0 )
                {
                    _nodeRead = node;
                }
                if ( _graph->getNbOutputConnections( *node ) == 0 )
                {
                    _nodeFinal = node;
                }
            }
        }
    }
    catch( ... )
    {
        TUTTLE_LOG_CURRENT_EXCEPTION;
    }
}

void VideoPlayer::buildGraph()
{
    using namespace tuttle::host;

    _nodeFinal = _nodeRead;

    TUTTLE_LOG_INFO( "Graph has been built" );
}

/**
 * @brief free all
 */
void VideoPlayer::terminate()
{
    std::unique_lock<std::mutex> lock( _mutexPlayer );
    if ( _graph )
    {
        _graph->clear();
        _graph.reset();
    }
}

/**
 * @brief is the player playing sound
 */
inline bool VideoPlayer::isPlaying() const
{
    return _playing;
}

/**
 * @brief sound volume control for the current played track
 * @param v volume value
 */
void VideoPlayer::setVolume( const float v )
{
    
}

/**
 * @brief mute sound
 */
void VideoPlayer::mute( const bool active )
{
    
}

/**
 * @brief load a given file
 * @param filename given file
 */
void VideoPlayer::load( const boost::filesystem::path & filename )
{
    if ( _nodeRead )
    {
        try
        {
            using namespace tuttle::host;
            _nodeRead->getParam( "filename" ).setValue( filename.string() );
            _inputSequence.reset();
            const OfxRangeD timeDomain = getTimeDomain();
            _currentPosition = timeDomain.min;
            _currentLength = timeDomain.max;
            _currentFPS = getFPS();
            signalPositionChanged( _currentPosition, _currentLength );
            signalTrackLength( _currentLength );
        }
        catch( ... )
        {
            TUTTLE_LOG_CURRENT_EXCEPTION;
        }
    }
}

/**
 * @brief frees the sound object
 */
void VideoPlayer::unload()
{
    std::unique_lock<std::mutex> lock( _mutexPlayer );
    signalEndOfTrack();
}

/**
 * @brief plays a sound
 * @param pause pause playing
 * @return false on success, true if error
 */
bool VideoPlayer::play( const bool pause )
{
    return false;
}

/**
 * @brief get a frame at a certain time
 * @return an image, null of error
 */
DefaultImageT VideoPlayer::getFrame( const double nFrame )
{
    try
    {
        std::unique_lock<std::mutex> lock( _mutexPlayer );
        DefaultImageT frame;
        _currentPosition = nFrame;
        _graph->compute( _outputCache, *_nodeFinal, tuttle::host::ComputeOptions( nFrame ) );
        return cache().get( _nodeFinal->getName(), nFrame );
    }
    catch( ... )
    {
        TUTTLE_LOG_CURRENT_EXCEPTION;
        return DefaultImageT();
    }
}

/**
 * @brief set output filename
 * @param nFrame[in] frame number
 * @param nbTotalFrames[in] total frame number
 * @param filePathPrefix[in] file path prefix
 * @param extenstion[in] file extension
 * @warning if the final node is not a writer, this will have no effect
 */
void VideoPlayer::setOutputFilename( const double nFrame, const std::size_t nbTotalFrames, const std::string & filePathPrefix, const std::string & extension )
{
    try
    {
        if ( _nodeRead != _nodeFinal )
        {
            auto & param = _nodeFinal->getParam( "filename" );
            std::ostringstream os;
            os << filePathPrefix;
            os.fill( '0' );
            os.width( std::ceil( std::log( nbTotalFrames ) / std::log( 10.0 ) ) );
            os << nFrame;
            os << "." << extension;
            param.setValue( os.str() );
        }
    }
    catch( ... )
    {}
}

/**
 * @brief initialize sequence
 * @param filePath[in] full file path
 */
void VideoPlayer::initSequence( const std::string & filePath )
{
    if ( !filePath.empty() )
    {
        _inputSequence.reset( new sequenceParser::Sequence( filePath ) );
        _inputSequence->initFromDetection( filePath, sequenceParser::Sequence::ePatternStandard );
        _frameStep = _inputSequence->getStep();
    }
    else
    {
        _inputSequence.reset();
        _frameStep = 1.0;
    }
}

/**
 * @brief get time domain definition
 * @return the time domain {min, max}
 */
OfxRangeD VideoPlayer::getTimeDomain() const
{
    assert( _nodeRead != nullptr );
    _graph->setup();
    if ( _inputSequence )
    {
        OfxRangeD timeDomain;
        timeDomain.min = _inputSequence->getFirstTime();
        timeDomain.max = _inputSequence->getLastTime();
        return timeDomain;
    }
    else
    {
        return _nodeRead->getTimeDomain();
    }
}

/**
 * @brief set current track position
 * @param position position in percent (0-100), ms or frames
 * @param seekType seek position in frame, percent or milliseconds
 * @return false on success, true if error
 */
bool VideoPlayer::setPosition( const std::size_t position, const mvpplayer::ESeekPosition seekType )
{
    // Set the right filename if playing a sequence
    if ( _inputSequence )
    {
        auto & param = _nodeRead->getParam( "filename" );
        const std::string inputSeqFilename = _inputSequence->getAbsoluteFilenameAt( position );
        param.setValue( inputSeqFilename );
    }

    switch( seekType )
    {
        case mvpplayer::eSeekPositionSample:
        {
            _currentPosition = position;
            signalPositionChanged( _currentPosition, _currentLength );
            return false;
        }
        case mvpplayer::eSeekPositionPercent:
        {
            _currentPosition = position * _currentLength;
            signalPositionChanged( _currentPosition, _currentLength );
            return false;
        }
        case mvpplayer::eSeekPositionMS:
        {
            _currentPosition = ( position / 1000.0 ) * _currentFPS;
            signalPositionChanged( _currentPosition, _currentLength );
            return false;
        }
    }
    return true;
}

/**
 * @brief get the current track's position
 * @return the current position in milliseconds
 */
std::size_t VideoPlayer::getPosition() const
{
    return _currentPosition;
}

/**
 * @brief get the current track's length
 * @return the current length in milliseconds
 */
std::size_t VideoPlayer::getLength() const
{
    return _currentLength;
}

/**
 * @brief restart track
 */
bool VideoPlayer::restart()
{
    ///@todo restart track
    return false;
}

/**
 * @brief checks whether the sound is on or off
 */
bool VideoPlayer::getSound()
{
    ///@todo get sound on/off
    return false;
}

/**
 * @brief pause or unpause the current track
 * @param pause boolean setting the pause
 */
void VideoPlayer::setPause( const bool pause )
{
    ///@todo pause reading thread
}

/**
 * @brief switch sound off/on
 * @param sound sound on/off
 */
void VideoPlayer::setSound( const bool sound )
{
    ///@todo set sound on
}

/**
 * @brief toggle sound on or off
 */
void VideoPlayer::toggleSound()
{
    ///@todo pause/unpause reading thread
}

/**
 * @brief toggle pause on or off
 */
void VideoPlayer::togglePause()
{
    ///@todo pause/unpause reading thread
}

/**
 * @brief is track paused
 */
bool VideoPlayer::isPaused() const
{
    return false;
}

}
