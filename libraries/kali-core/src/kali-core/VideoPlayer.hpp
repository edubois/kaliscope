#ifndef _KALI_CORE_VIDEOCAPTURE_HPP_
#define	_KALI_CORE_VIDEOCAPTURE_HPP_

#include "typedefs.hpp"

#include <mvp-player-core/IVideoPlayer.hpp>
#include <mvp-player-core/IFilePlayer.hpp>
#include <mvp-player-core/Singleton.hpp>

#include <tuttle/common/utils/global.hpp>
#include <tuttle/host/Graph.hpp>
#include <ofxCore.h>

namespace kaliscope
{

class VideoPlayer : public mvpplayer::IVideoPlayer, public mvpplayer::Singleton<VideoPlayer>
{
public:
    VideoPlayer( const std::shared_ptr<tuttle::host::Graph> & graph = std::shared_ptr<tuttle::host::Graph>() );
    virtual ~VideoPlayer();

// Overrides
public:
    /**
     * @brief get memory cache
     */
    inline tuttle::host::memory::MemoryCache & cache()
    { return _outputCache; }

    /**
     * @brief get time domain definition
     * @return the time domain {min, max}
     */
    OfxRangeD getTimeDomain() const
    {
        assert( _nodeRead != nullptr );
        return _nodeRead->getTimeDomain();
    }

    /**
     * @brief get the frames per seconds
     * @return the fps
     */
    double getFPS() const
    {
        assert( _nodeRead != nullptr );
        return 24;  ///<@todo get fps
    }

    /**
     * @brief initialize all
     */
    void initialize();

    /**
     * @brief build processing graph
     */
    void buildGraph();
    
    /**
     * @brief free all
     */
    void terminate();

    /**
     * @brief is the player playing sound
     */
    inline bool isPlaying() const override;

    /**
     * @brief sound volume control for the current played track
     * @param v volume value
     */
    void setVolume( const float v ) override;
    
    /**
     * @brief mute sound
     */
    void mute( const bool active = true ) override;

    /**
     * @brief load a given file
     * @param filename given file
     */
    void load( const boost::filesystem::path & filename ) override;

    /**
     * @brief frees the sound object
     */
    void unload() override;

    /**
     * @brief plays a sound
     * @param pause pause playing
     * @return false on success, true if error
     */
    bool play (const bool pause = false) override;

    /**
     * @brief get a frame at a certain time
     * @param nFrame frame number in time domain
     * @return an image, null of error
     */
    virtual DefaultImageT getFrame( const double nFrame );

    /**
     * @brief get current frame at a certain time
     * @return an image, null of error
     */
    DefaultImageT getFrame()
    { return getFrame( _currentPosition ); }

    /**
     * @brief set current track position
     * @param position position in percent (0-100) or ms
     * @param seekType seek position in frame, percent or milliseconds
     * @return false on success, true if error
     */
    bool setPosition( const std::size_t positionInPercent, const mvpplayer::ESeekPosition seekType = mvpplayer::eSeekPositionSample ) override;

    /**
     * @brief get the current track's position
     * @return the current position in milliseconds
     */
    std::size_t getPosition() const override;

    /**
     * @brief get the current track's length
     * @return the current length in milliseconds
     */
    std::size_t getLength() const override;

    /**
     * @brief restart track
     */
    bool restart() override;

    /**
     * @brief checks whether the sound is on or off
     */
    bool getSound();

    /**
     * @brief pause or unpause the current track
     * @param pause boolean setting the pause
     */
    void setPause( const bool pause ) override;

    /**
     * @brief switch sound off/on
     * @param sound sound on/off
     */
    void setSound( const bool sound ) override;

    /**
     * @brief toggle sound on or off
     */
    void toggleSound() override;

    /**
     * @brief toggle pause on or off
     */
    void togglePause() override;

    /**
     * @brief is track paused
     */
    bool isPaused() const override;

// Various
private:
    double _currentPosition = 0.0;      //< Current track position
    double _currentLength = 0.0;        //< Current track length
    double _currentFPS = 0.0;           //< Current frames per seconds
    bool _playing = false;              ///< 'Is playing track' status

// Thread related
private:
    boost::mutex _mutexPlayer;                              ///< Mutex thread

// TuttleOFX related
private:
    tuttle::host::Graph::Node *_nodeFinal = nullptr;        ///< Final effect node
    tuttle::host::Graph::Node *_nodeRead = nullptr;         ///< File reader
    std::shared_ptr<tuttle::host::Graph> _graph;                ///< effects processing graph
    tuttle::host::memory::MemoryCache _outputCache;         ///< Cache for video output
};

}

#endif
