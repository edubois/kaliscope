#include "GpioWatcher.hpp"

#include <mvp-player-core/Settings.hpp>

#include <boost/format.hpp>
#include <chrono>

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

namespace kaliscope
{

/**
 * @brief constructor
 * @param numPin pin number
 * @param microsecDelay wait delay if -1, no watching thread will be started
 */
GpioWatcher::GpioWatcher( const std::size_t pinId, const int microsecDelay )
: _gpioId( pinId )
{
    if ( microsecDelay >= 0 )
    {
        startWatching( microsecDelay );
    }
}

GpioWatcher::~GpioWatcher()
{
    stop();
}

void GpioWatcher::toggleValue()
{
    _value = !_value;
    setValGpio( _value );
}

bool GpioWatcher::exportGpio()
{
    static const std::string exportStr( "/sys/class/gpio/export" );
    // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    std::ofstream exportgpio( exportStr.c_str() );
    if ( !exportgpio.good() )
    {
        std::cerr << "OPERATION FAILED: Unable to export GPIO " << _gpioId << " ." << std::endl;
        return false;
    }

    // write GPIO number to export
    exportgpio << _gpioId ;
    // close export file
    exportgpio.close();
    return true;
}

bool GpioWatcher::unexportGpio()
{
    static const std::string unexportStr( "/sys/class/gpio/unexport" );
    std::ofstream unexportgpio( unexportStr.c_str() ); //Open unexport file
    if ( !unexportgpio.good() )
    {
        std::cerr << "OPERATION FAILED: Unable to unexport GPIO " << _gpioId << " ." << std::endl;
        return false;
    }

    // Write GPIO number to unexport
    unexportgpio << _gpioId;
    // Close unexport file
    unexportgpio.close();
    return true;
}

bool GpioWatcher::setDirGpio( const std::string & dir )
{
    const std::string setdirStr = ( boost::format( "/sys/class/gpio/gpio%1%/direction" ) % _gpioId ).str();
    // open direction file for gpio
    std::ofstream setdirgpio( setdirStr.c_str() );
    if ( !setdirgpio.good() )
    {
        std::cerr << "OPERATION FAILED: Unable to set direction of GPIO " << _gpioId << "."<< std::endl;
        return false;
    }

    setdirgpio << dir; //write direction to direction file
    setdirgpio.close(); // close direction file
    return true;
}

/**
 * @brief set gpio value
 * @param value true or false
 * @return false if failure, true otherwise
 */
bool GpioWatcher::setValGpio( const bool value )
{
    const std::string setValStr = ( boost::format( "/sys/class/gpio/gpio%1%/value" ) % _gpioId ).str();
    std::ofstream setvalgpio( setValStr.c_str() ); // open value file for gpio

    if ( !setvalgpio.good() )
    {
        std::cerr << "OPERATION FAILED: Unable to set the value of GPIO "<< _gpioId << "." << std::endl;
        return false;
    }

    // write boolean value to value file
    setvalgpio << value;
    _value = value;
    // close value file
    setvalgpio.close();
    return true;
}

/**
 * @brief get gpio value
 * @param val[out] output value
 * @return false if failure, true otherwise
 */
bool GpioWatcher::getValGpio( bool & val )
{
    const std::string getValStr = ( boost::format( "/sys/class/gpio/gpio%1%/value" ) % _gpioId ).str();
    std::ifstream getvalgpio( getValStr.c_str() );// open value file for gpio
    if ( !getvalgpio.good() )
    {
        std::cerr << "OPERATION FAILED: Unable to get value of GPIO " << _gpioId << "." << std::endl;
        return false;
    }

    // read gpio boolean value
    getvalgpio >> val;

    // Invert boolean value
    val = !val;

    // Close the value file
    getvalgpio.close();
    return true;
}

void GpioWatcher::worker()
{
    if ( _stop )
    { return; }

    bool lastValue = false;
    _stop = !getValGpio( lastValue );
    signalGpioValueChanged( _gpioId, lastValue );

    while( !_stop )
    {
        std::this_thread::sleep_for( std::chrono::microseconds( _microsecDelay ) );
        bool gpioValue = false;
        if ( getValGpio( gpioValue ) )
        {
            if ( lastValue != gpioValue )
            {
                signalGpioValueChanged( _gpioId, gpioValue );
                lastValue = gpioValue;
                // Avoid parasites
                std::this_thread::sleep_for( std::chrono::milliseconds( mvpplayer::Settings::getInstance().get<int>( "gpio", "nextEventDelay" ) ) );
            }
        }
        else
        {
            _stop = true;
        }
    }
}

/**
 * @brief start watching thread
 * @param microsecDelay microseconds delay between each reading
 */
void GpioWatcher::startWatching( const std::size_t microsecDelay )
{
    _microsecDelay = microsecDelay;
    exportGpio();
    setDirGpio( "in" );
    _stop = false;
    _watcherThread.reset( new std::thread( &This::worker, this ) );
}

/**
 * @brief stop watching thread
 */
void GpioWatcher::stop()
{
    _stop = true;
    _watcherThread->join();
    unexportGpio();
}

}
