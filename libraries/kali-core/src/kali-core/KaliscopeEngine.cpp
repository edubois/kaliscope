#include "KaliscopeEngine.hpp"

#include <boost/algorithm/string/predicate.hpp>

namespace kaliscope
{

KaliscopeEngine::KaliscopeEngine( VideoPlayer *videoPlayer )
: Parent( videoPlayer )
, _videoPlayer( videoPlayer )
{
    assert( _videoPlayer != nullptr );
}

KaliscopeEngine::~KaliscopeEngine()
{
    stop();
    terminate();
}

/**
 * @brief set processing graph
 * @param graph new processing graph
 * @return previous processing graph
 */
std::shared_ptr<tuttle::host::Graph> KaliscopeEngine::setProcessingGraph( const std::shared_ptr<tuttle::host::Graph> & graph )
{
    stop();
    return _videoPlayer->setProcessingGraph( graph );
}

/**
 * @brief used to read video step by step
 */
void KaliscopeEngine::playWork()
{
    _stopped = false;
    try
    {
        using namespace tuttle::host;
        DefaultImageT image;

        if ( !_inputFilePath.empty() )
        {
            _videoPlayer->setInputFilename( _inputFilePath.string(), _isInputSequence );
        }

        if ( !_isOutputSequence && !_outputFilePathPrefix.empty() )
        {
            _videoPlayer->setOutputFilename( _outputFilePathPrefix );
        }

        const OfxRangeD timeDomain = _videoPlayer->getTimeDomain();
        const double step = _videoPlayer->getFrameStep();

        std::cout << "Time domain: {" << timeDomain.min << "," << timeDomain.max << "}" << std::endl;
        if ( timeDomain.min == timeDomain.max )
        {
            TUTTLE_LOG_INFO( "Video is empty!" );
        }

        _semaphoreSynchro.takeAll();
        _semaphoreFrameStepping.takeAll();
        for( double nFrame = timeDomain.min; nFrame <= timeDomain.max && !_stopped; nFrame += step )
        {
            image.reset();

            try
            {
                boost::this_thread::interruption_point();
                if ( !_stopped )
                {
                    _videoPlayer->setPosition( nFrame, mvpplayer::eSeekPositionSample );
                    if ( _isOutputSequence )
                    {
                        _videoPlayer->setOutputFilename( nFrame, std::ceil( timeDomain.max ), _outputFilePathPrefix, _outputFileExtension );
                    }
                    image = _videoPlayer->getFrame();
                }
                else
                {
                    std::cout << "Video player stopped" << std::endl;
                    break;
                }
            }
            catch( ... )
            {
                TUTTLE_LOG_CURRENT_EXCEPTION;
            }
            if ( !_stopped )
            {
                if ( image )
                {
                    signalFrameReady( nFrame, image );
                }
                else
                {
                    std::cerr << "Unable to read frame!" << std::endl;
                    break;
                }
                _semaphoreSynchro.wait();
                if ( _frameStepping )
                {
                    _semaphoreFrameStepping.wait();
                }
            }
        }
    }
    catch( ... )
    {
        TUTTLE_LOG_CURRENT_EXCEPTION;
    }

    _videoPlayer->unload();
    _stopped = true;
}

/**
 * @brief stop playing
 */
void KaliscopeEngine::stop()
{
    stopWorker();
}

/**
 * @brief stop worker thread
 */
void KaliscopeEngine::stopWorker()
{
    if ( _playerThread )
    {
        try
        {
            _stopped = true;
            _semaphoreSynchro.post();
            _semaphoreFrameStepping.post();
            if ( _playerThread->joinable() )
            {
                _playerThread->join();
            }
        }
        catch( ... )
        {
        }
        _playerThread.reset();
    }
}

/**
 * @brief tells the engine that a has been processed,
 *         and that we can process the next one
 * @param nFrame frame number
 */
void KaliscopeEngine::frameProcessed( const double nFrame )
{
    _semaphoreSynchro.post();
}

/**
 * @brief start processing thread
 */
void KaliscopeEngine::start()
{
    stop();
    _playerThread.reset( new std::thread( &KaliscopeEngine::playWork, this ) );
}

/**
 * @brief play a given file
 * @return false on success, true if error
 */
bool KaliscopeEngine::playFile( const boost::filesystem::path & filename )
{
    stop();
    _isInputSequence = false;

    if ( boost::iends_with( filename.string(), ".m3u" ) )
    {
        openPlaylist( filename );
        playList();
        return true;
    }
    else
    {
        try
        {
            {
                std::unique_lock<std::mutex> lock( _mutexPlayer );

                _currentPlayedTrack = filename;
                _filePlayer->load( filename );
                signalPlayedTrack( filename );
            }
            start();
            return true;
        }
        catch( ... )
        {
            TUTTLE_LOG_CURRENT_EXCEPTION;
            return false;
        }
    }
}

}

