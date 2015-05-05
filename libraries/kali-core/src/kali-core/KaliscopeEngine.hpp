#ifndef _KALISCOPEENGINE_HPP_
#define	_KALISCOPEENGINE_HPP_

#include "typedefs.hpp"
#include "VideoPlayer.hpp"

#include <mvp-player-core/MVPPlayerEngine.hpp>

#include <boost/gil/image_view.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/thread.hpp>

#include <thread>

namespace kaliscope
{

/**
 * @brief kaliscope engine
 */
class KaliscopeEngine : public mvpplayer::MVPPlayerEngine
{
private:
    typedef mvpplayer::MVPPlayerEngine Parent;
    typedef KaliscopeEngine This;
public:
    KaliscopeEngine( VideoPlayer *videoPlayer );
    virtual ~KaliscopeEngine();

    /**
     * @brief play a given file
     * @return false on success, true if error
     */
    bool playFile( const boost::filesystem::path & filename ) override;
    
    /**
     * @brief tells the engine that a has been processed,
     *         and that we can process the next one
     * @param nFrame frame number
     */
    void frameProcessed( const double nFrame );

private:

    /**
     * @brief stop playing
     */
    void stop();

    /**
     * @brief stop worker thread
     */
    void stopWorker();

    /**
     * @brief used to read video step by step
     */
    void playWork();

// Signals
public:
    boost::signals2::signal<void( const std::size_t nFrame, const DefaultImageT image )> signalFrameReady;   ///< Signals that a new frame is ready

// Various
private:
    VideoPlayer *_videoPlayer = nullptr;                ///< Pointer to the video player
    bool _stopped = false;
    double _processFrame = -1.0;                        ///< Process a given frame

// Thread related
private:
    std::mutex _mutexPlayer;                            ///< Mutex thread
    std::mutex _mutexSynchro;                           ///< Mutex thread
    std::condition_variable _synchroCondition;          ///< Synchronization condition
    std::unique_ptr<std::thread> _playerThread;       ///< Player's thread
};

}

#endif
