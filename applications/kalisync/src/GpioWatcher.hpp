#ifndef _KALI_GPIOWATCHER_HPP_
#define	_KALI_GPIOWATCHER_HPP_

#include <boost/signals2.hpp>

#include <string>
#include <memory>
#include <thread>

namespace kaliscope
{

/**
 * @brief GPIO Watcher
 * Used to get IO from GPIO pins
 * tested on raspberry pi 2
 */
class GpioWatcher
{
    typedef GpioWatcher This;
public:
    /**
     * @brief constructor
     * @param numPin pin number
     * @param microsecDelay wait delay if -1, no watching thread will be started
     */
    GpioWatcher( const std::size_t numPin, const int microsecDelay = -1 );

    virtual ~GpioWatcher();

    /**
     * @brief get the gpio pin id
     * @return the pin id
     */
    inline const std::size_t gpioId() const
    { return _gpioId; }

    /**
     * @brief start watching thread
     * @param microsecDelay microseconds delay between each reading
     */
    void startWatching( const std::size_t microsecDelay = 0 );

    /**
     * @brief stop watching thread
     */
    void stop();

    /**
     * @brief export gpio for further use
     * @return false if failure, true otherwise
     */
    bool exportGpio();

    /**
     * @brief unexport (release) gpio
     * @return false if failure, true otherwise
     */
    bool unexportGpio();

    /**
     * @brief set gpio direction
     * @param dir[in] { out, in }
     * @return false if failure, true otherwise
     */
    bool setDirGpio( const std::string & dir );

    /**
     * @brief set gpio value
     * @param value true or false
     * @return false if failure, true otherwise
     */
    bool setValGpio( const bool value ); // Set GPIO Value (putput pins)

    /**
     * @brief toggle gpio value
     */
    void toggleValue();

    /**
     * @brief get gpio value
     * @param val[out] output value
     * @return false if failure, true otherwise
     */
    bool getValGpio( bool & val );

protected:
    /**
     * @brief watching work
     */
    virtual void worker();

/**
 * @brief signals
 */
public:
    boost::signals2::signal<void( const std::size_t pinNum, const bool value )> signalGpioValueChanged; ///< Signalize that the GPIO value has changed

private:
    bool _value = false;            ///< Value
    const std::size_t _gpioId;      ///< GPIO number associated with the instance of an object
    std::unique_ptr<std::thread> _watcherThread;       ///< Watcher's thread
    bool _stop = true;              ///< Stops watcher thread
    std::size_t _microsecDelay = 0; ///< microsec delay between each gpio operation
};

}

#endif
