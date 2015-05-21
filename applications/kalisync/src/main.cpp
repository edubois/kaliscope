#include "GpioWatcher.hpp"
#include <mvp-player-net/server/Server.hpp>
#include <mvp-player-core/stateMachineEvents.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/exception/all.hpp>

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

static const char * kServerPortOptionString( "port" );
static const char * kServerPortOptionMessage( "Port for network server" );
static const char * kWatchInputPinOptionString( "watch" );
static const char * kWatchInputPinOptionMessage( "Watch input pin (gpio id)" );
static const char * kLedPinOptionString( "ledPin" );
static const char * kLedPinOptionMessage( "Led output pin (gpio id)" );

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

void captureTriggered( mvpplayer::network::server::Server & server, const bool pinValue )
{
    if ( pinValue )
    {
        // Send next track
        mvpplayer::logic::EvNextTrack event;
        std::cout << "Next frame!" << std::endl;
	server.sendEventMulticast( event );
    }
}

int main( int argc, char** argv )
{
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

    std::set_terminate( &kalisync_terminate );
    std::set_unexpected( &kalisync_unexpected );
    try
    {
        // Declare the supported options.
        bpo::options_description mainOptions( "Allowed options" );
        mainOptions.add_options()
            ( kServerPortOptionString,  bpo::value<unsigned short>()->default_value( mvpplayer::network::server::kDefaultServerPort ), kServerPortOptionMessage )
            ( kLedPinOptionString, bpo::value<int>()->required(), kLedPinOptionMessage )
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

        GpioWatcher gpioWatcher( vm[kWatchInputPinOptionString].as<int>(), 0 );
        GpioWatcher gpioLedFrameDone( vm[kLedPinOptionString].as<int>() );
        gpioLedFrameDone.exportGpio();
        gpioLedFrameDone.setDirGpio( "out" );

        std::cout << "[Kalisync] GPIO Watcher started..." << std::endl;
        Server server( vm[kServerPortOptionString].as<unsigned short>() );
        server.run();
        std::cout << "[Kalisync] GPIO Server started..." << std::endl;
        gpioWatcher.signalGpioValueChanged.connect( boost::bind( &captureTriggered, boost::ref( server ), _2 ) );
        // Blink led
        gpioWatcher.signalGpioValueChanged.connect( boost::bind( &GpioWatcher::setValGpio, &gpioLedFrameDone, true ) );
        gpioWatcher.signalGpioValueChanged.connect( boost::bind( &GpioWatcher::setValGpio, &gpioLedFrameDone, false ) );
        server.wait();
    }
    catch( ... )
    {
        std::cerr << "[Kalisync] Exception ... : main of Kalisync." << std::endl;
        std::cerr << boost::current_exception_diagnostic_information() << std::endl;
        return -1;
    }

    return 0;
}

