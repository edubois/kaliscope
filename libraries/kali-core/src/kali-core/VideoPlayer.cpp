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
//    terminate();
}

/**
 * @brief initialize all
 */
void VideoPlayer::initialize()
{
    using namespace tuttle::host;
    try
    {
        if ( !_graph )
        {
            _graph.reset( new tuttle::host::Graph() );
            _nodeRead = &_graph->createNode( "tuttle.avreader" );
            // We want an rgb output
            _nodeRead->getParam( "channel" ).setValue( 2 );
            _nodeRead->getParam( "bitDepth" ).setValue( 1 );

            buildGraph();
        }
        else
        {
            std::vector<Graph::Node*> nodes = _graph->getNodes();
            if ( nodes.size() )
            {
                _nodeRead = nodes[0];
                _nodeFinal = nodes[nodes.size()-1];
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
            _graph->setup();
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
 * @brief set current track position
 * @param position position in percent (0-100), ms or frames
 * @param seekType seek position in frame, percent or milliseconds
 * @return false on success, true if error
 */
bool VideoPlayer::setPosition( const std::size_t position, const mvpplayer::ESeekPosition seekType )
{
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
