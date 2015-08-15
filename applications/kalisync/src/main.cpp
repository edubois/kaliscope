#include "GpioWatcher.hpp"
#include "projector/IProjector.hpp"
#include "projector/TinyDisplayProjector.hpp"

#include <kali-core/stateMachineEvents.hpp>

#include <mvp-player-net/server/Server.hpp>
#include <mvp-player-core/stateMachineEvents.hpp>
#include <mvp-player-core/Settings.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/exception/all.hpp>
#include <thread>
#include <chrono>

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

static const char * kServerPortOptionString( "port" );
static const char * kServerPortOptionMessage( "Port for network server" );
static const char * kWatchInputPinOptionString( "watch" );
static const char * kWatchInputPinOptionMessage( "Watch input pin (gpio id)" );
static const char * kMotorPinOptionString( "motorPin" );
static const char * kMotorPinOptionMessage( "Motor pin (gpio id)" );
static const char * kFlashPinOptionString( "flashPin" );
static const char * kFlashPinOptionMessage( "Flash pin (gpio id)" );
static const char * kGpioDelayOptionString( "gpioDelay" );
static const char * kGpioDelayOptionMessage( "Next gpio event delay (40 is a good value)" );
static const char * kUseTinyDisplayOptionString( "useTinyDisplay" );
static const char * kUseTinyDisplayOptionMessage( "Use tiny display as projector (need FBTFT driver)" );

mvpplayer::network::server::Server * pServer = NULL;

void signal_interrupt_handler( const int )
{
    if ( pServer )
    {
        // Asks the server to stop
        pServer->stop();
    }
    else
    {
        exit( -1 );
    }
}

void kalisync_terminate( void )
{
    std::cerr << "[Kalisync] Sorry, Kalisync has encountered a fatal error." << std::endl;
    std::cerr << "[Kalisync] Please report this bug." << std::endl;
    exit( -1 );
}

void kalisync_unexpected( void )
{
    std::cerr << "[Kalisync] Sorry, Kalisync has encountered a fatal error." << std::endl;
    std::cerr << "[Kalisync] Please report this bug." << std::endl;
    BOOST_THROW_EXCEPTION( std::runtime_error( "Sorry, Kalisync has encountered an unexpected exception.\nPlease report this bug." ) );
}

inline void triggerCapture( mvpplayer::network::server::Server & server )
{
    // Send next track event (means next frame in kaliscope)
    mvpplayer::logic::EvNextTrack event;
    server.sendEventMulticast( event );
}

int main( int argc, char** argv )
{
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

    std::set_terminate( &kalisync_terminate );
    std::set_unexpected( &kalisync_unexpected );
    // React to CTRL+C by stopping the server
    signal( SIGINT, signal_interrupt_handler );

    try
    {
        // Declare the supported options.
        bpo::options_description mainOptions( "Allowed options" );
        mainOptions.add_options()
            ( kServerPortOptionString,  bpo::value<unsigned short>()->default_value( mvpplayer::network::server::kDefaultServerPort ), kServerPortOptionMessage )
            ( kMotorPinOptionString, bpo::value<int>()->required(), kMotorPinOptionMessage )
            ( kFlashPinOptionString, bpo::value<int>()->required(), kFlashPinOptionMessage )
            ( kGpioDelayOptionString, bpo::value<int>()->required(), kGpioDelayOptionMessage )
            ( kUseTinyDisplayOptionString, bpo::value<bool>()->required()->default_value( true ), kUseTinyDisplayOptionMessage )
            ( kWatchInputPinOptionString, bpo::value<int>()->required(), kWatchInputPinOptionMessage );

        //parse the command line, and put the result in vm
        bpo::variables_map vm;
        bpo::store( bpo::parse_command_line( argc, argv, mainOptions ), vm );
        
        if ( argc == 1 )
        {
            std::cout << mainOptions << std::endl;
            std::cout << "Warning: this program must be run in root mode (GPIO requires that)." << std::endl;
            return 2;
        }
        
        bpo::notify(vm);

        using namespace kaliscope;
        using namespace mvpplayer;
        using namespace mvpplayer::network::server;

        mvpplayer::Settings::getInstance().set( "gpio", "nextEventDelay", vm[kGpioDelayOptionString].as<int>() );

        std::unique_ptr<IProjector> projector;
        if ( vm[kUseTinyDisplayOptionString].as<bool>() )
        {
            projector.reset( new TinyDisplayProjector() );
        }
        GpioWatcher gpioWatcher( vm[kWatchInputPinOptionString].as<int>(), 0 );
        GpioWatcher gpioMotor( vm[kMotorPinOptionString].as<int>() );
        gpioMotor.exportGpio();
        gpioMotor.setDirGpio( "out" );
        gpioMotor.setValGpio( false );
        GpioWatcher gpioFlash( vm[kFlashPinOptionString].as<int>() );
        gpioFlash.exportGpio();
        gpioFlash.setDirGpio( "out" );
        gpioFlash.setValGpio( false );

        std::cout << "[Kalisync] GPIO Watcher started..." << std::endl;
        Server server( vm[kServerPortOptionString].as<unsigned short>() );
        server.run();
        pServer = &server;
        std::cout << "[Kalisync] GPIO Server started..." << std::endl;
        if ( projector )
        {
            std::cout << "Testing projector..." << std::endl;
            projector->switchOn();
            std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
            projector->switchOff();
            std::cout << "Projector tested." << std::endl;
        }

        // Toggle led value
        gpioWatcher.signalGpioValueChanged.connect(
            [&server, &gpioMotor, &gpioFlash, &projector]( const std::size_t, const bool value )
            {
                if ( value == true )
                {
                    // Stop the motor and light the flash
                    gpioMotor.setValGpio( false );
                    gpioFlash.setValGpio( true );
                    if ( projector )
                    { projector->switchOn(); }
                    // Ask the client to capture a frame
                    triggerCapture( server );
                }
            }
        );

        server.signalEventFrom.connect(
            [&gpioFlash, &projector, &gpioMotor](const std::string&, IEvent& event)
            {
                using namespace mvpplayer::logic;
                // When a frame has been captured, we want to step forward
                if ( dynamic_cast<mvpplayer::logic::EvCustomState*>( &event ) )
                {
                    const EvCustomState& customState = dynamic_cast<EvCustomState&>( event );
                    if ( customState.action() == kaliscope::kFrameCapturedCustomStateAction )
                    {
                        // Stop the flash light and restart the motor
                        gpioFlash.setValGpio( false );
                        if ( projector )
                        { projector->switchOff(); }
                        gpioMotor.setValGpio( true );
                    }
                    else if ( customState.action() == kaliscope::kCaptureStopCustomStateAction )
                    {
                        // Stop the flash light and the motor
                        gpioFlash.setValGpio( false );
                        if ( projector )
                        { projector->switchOff(); }
                        gpioMotor.setValGpio( false );
                    }
                }
                // When we hit stop, we want to stop flash and motor
                else if ( dynamic_cast<EvStop*>( &event ) )
                {
                    gpioFlash.setValGpio( false );
                    gpioMotor.setValGpio( false );
                }
            }
        );
        server.wait();
        gpioFlash.setValGpio( false );
        if ( projector )
        { projector->switchOff(); }
        gpioMotor.setValGpio( false );
    }
    catch( ... )
    {
        std::cerr << "[Kalisync] Exception ... : main of Kalisync." << std::endl;
        std::cerr << boost::current_exception_diagnostic_information() << std::endl;
        return -1;
    }

    return 0;
}

