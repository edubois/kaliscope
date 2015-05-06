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
        const OfxRangeD timeDomain = _videoPlayer->getTimeDomain();
        std::cout << "Time domain: {" << timeDomain.min << "," << timeDomain.max << "}" << std::endl;
        if ( timeDomain.min == timeDomain.max )
        {
            TUTTLE_LOG_INFO( "Video is empty!" );
        }
        _processFrame = timeDomain.min;
        std::unique_lock<std::mutex> synchro( _mutexSynchro );
        for( double nFrame = timeDomain.min; nFrame < timeDomain.max && !_stopped; ++nFrame )
        {
            image.reset();
            try
            {
                boost::this_thread::interruption_point();
                if ( !_stopped )
                {
                    _videoPlayer->setPosition( nFrame, mvpplayer::eSeekPositionSample );
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

            if ( image )
            {
                std::cout << "Frame " << nFrame << " is ready!" << std::endl;
                signalFrameReady( nFrame, image );
            }
            else
            {
                std::cerr << "Unable to read frame!" << std::endl;
                break;
            }
            _synchroCondition.wait( synchro );
        }
    }
    catch( boost::thread_interrupted& )
    {}
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
            _synchroCondition.notify_all();
            _playerThread->join();
        }
        catch( ... )
        {}
    }
}

/**
 * @brief tells the engine that a has been processed,
 *         and that we can process the next one
 * @param nFrame frame number
 */
void KaliscopeEngine::frameProcessed( const double nFrame )
{
    _processFrame = nFrame+1;
    _synchroCondition.notify_all();
}

/**
 * @brief play a given file
 * @return false on success, true if error
 */
bool KaliscopeEngine::playFile( const boost::filesystem::path & filename )
{
    stop();

    if ( boost::iends_with( filename.string(), ".m3u" ) )
    {
        openPlaylist( filename );
        playList();
        return false;
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
            _playerThread.reset( new std::thread( &KaliscopeEngine::playWork, this ) );
            return false;
        }
        catch( ... )
        {
            TUTTLE_LOG_CURRENT_EXCEPTION;
            return true;
        }
    }
}

}

