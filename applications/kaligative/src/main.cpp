#include <tuttle/common/utils/global.hpp>
#include <tuttle/host/Graph.hpp>
#include <Sequence.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

static const char * kInputPathOptionString( "input" );
static const char * kInputPathOptionMessage( "Input sequence filename pattern" );
static const char * kOutputPathOptionString( "output" );
static const char * kOutputPathOptionMessage( "Ouput sequence prefix" );
static const char * kLutPathOptionString( "lut" );
static const char * kLutPathOptionMessage( "Lut filename (.3dl)" );
static const char * kInvertOptionMessage( "Invert image" );
static const char * kInvertOptionString( "invert" );
static const bool kInvertOptionDefaultValue( false );

void kaligative_terminate( void )
{
	TUTTLE_LOG_ERROR( "[Kaligative example] Sorry, Kaligative has encountered a fatal error." );
	TUTTLE_LOG_ERROR( "[io example] Please report this bug." );
	exit( -1 );
}

void kaligative_unexpected( void )
{
	TUTTLE_LOG_ERROR( "[Kaligative example] Sorry, Kaligative has encountered a fatal error." );
	TUTTLE_LOG_ERROR( "[Kaligative example] Please report this bug." );
	BOOST_THROW_EXCEPTION( std::runtime_error( "Sorry, Sam has encountered an unexpected exception.\nPlease report this bug." ) );
}

void processGraphOnSequence( const sequenceParser::Sequence & s, tuttle::host::Graph & g, tuttle::host::Graph::Node& reader, tuttle::host::Graph::Node& writer, const std::string & outputPrefix )
{
    using namespace tuttle::host;

    const std::ssize_t first = s.getFirstTime();
    const std::ssize_t last = s.getLastTime();
    for( sequenceParser::Time t = first; t <= last; t += s.getStep() )
    {
            bfs::path sFile = s.getAbsoluteFilenameAt(t);

            if( !bfs::exists( sFile ) )
            {
                    TUTTLE_LOG_ERROR( "Could not remove (file not exist): " << sFile.string() );
            }
            else
            {
                TUTTLE_LOG_INFO( "[Kaligative] processing frame '" << t << "'" );
                reader.getParam( "filename" ).setValue( sFile.string() );
                writer.getParam( "filename" ).setValue( ( boost::format( "%1%_%2%" ) % outputPrefix % t ).str() );
                g.compute( writer );
            }
    }
}

int main( int argc, char** argv )
{
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );
    tuttle::common::Formatter::get();

    std::set_terminate( &kaligative_terminate );
    std::set_unexpected( &kaligative_unexpected );
    try
    {
        // Declare the supported options.
        bpo::options_description mainOptions( "Allowed options" );
        mainOptions.add_options()
                        ( kInputPathOptionString,  bpo::value<std::string>()->required(), kInputPathOptionMessage )
                        ( kOutputPathOptionString, bpo::value<std::string>()->required(), kOutputPathOptionMessage )
                        ( kLutPathOptionString,    bpo::value<std::string>(), kLutPathOptionMessage )
                        ( kInvertOptionString, bpo::value<bool>()->default_value( kInvertOptionDefaultValue ), kInvertOptionMessage );

        //parse the command line, and put the result in vm
        bpo::variables_map vm;
        bpo::store(bpo::parse_command_line(argc, argv, mainOptions), vm);
        
        if ( argc == 1 )
        {
            TUTTLE_COUT( mainOptions );
            return 2;
        }
        
        bpo::notify(vm);

        using namespace tuttle::host;
        TUTTLE_LOG_INFO( "[Kaligative] Preload plugins" );
        core().preload();

        TUTTLE_TLOG( TUTTLE_INFO, core().getImageEffectPluginCache() );

        TUTTLE_LOG_INFO( "[Kaligative] Preload done" );

        TUTTLE_LOG_INFO( "[Kaligative] create plugins" );
        Graph g;
        Graph::Node& read1   = g.createNode( "tuttle.oiioreader" );
        Graph::Node& bitdepth1 = g.createNode( "tuttle.bitdepth" );
        Graph::Node& invert1 = g.createNode( "tuttle.invert" );
        Graph::Node& lut1   = g.createNode( "tuttle.lut" );
        Graph::Node& write1    = g.createNode( "tuttle.dpxwriter" );

        TUTTLE_LOG_INFO( "[Kaligative] set plugins parameters" );
        // Setup parameters
        if ( vm.count( kLutPathOptionString ) )
        {
            lut1.getParam( "filename" ).setValue( vm[kLutPathOptionString].as<std::string>() );
        }

        TUTTLE_LOG_INFO( "[Kaligative] connect plugins" );
        g.connect( read1, bitdepth1 );
        Graph::Node *last = nullptr; 

        if ( vm[kInvertOptionString].as<bool>() )
        {
            g.connect( bitdepth1, invert1 );
            last = &invert1;
        }
        else
        {
            last = &bitdepth1;
        }

        if ( vm.count( kLutPathOptionString ) )
        {
            g.connect( *last, lut1 );
            last = &lut1;
        }

        g.connect( *last, write1 );

        TUTTLE_LOG_INFO( "[Kaligative] processing sequence" );

        const bfs::path inputPath = vm[kInputPathOptionString].as<std::string>();
        try
        {
            sequenceParser::Sequence s( inputPath );
            s.initFromDetection( vm[kInputPathOptionString].as<std::string>(), sequenceParser::Sequence::ePatternStandard );
            if( s.getNbFiles() )
            {
                try
                {
                    processGraphOnSequence( s, g, read1, write1, vm[kOutputPathOptionString].as<std::string>() );
                }
                catch(... )
                {
                    TUTTLE_LOG_CURRENT_EXCEPTION;
                }
            }
        }
        catch(... )
        {
            TUTTLE_LOG_CURRENT_EXCEPTION;
            TUTTLE_LOG_ERROR( "Unrecognized pattern \"" << inputPath << "\"" );
        }

        TUTTLE_LOG_INFO( "[simpleCapture example] finish" );
    }
    catch( tuttle::exception::Common& e )
    {
            TUTTLE_LOG_ERROR( "Tuttle Exception : main of Kaligative." );
            TUTTLE_LOG_ERROR( boost::diagnostic_information( e ) );
            return -1;
    }
    catch(... )
    {
            TUTTLE_LOG_ERROR( "Exception ... : main of Kaligative." );
            TUTTLE_LOG_CURRENT_EXCEPTION;
            return -1;
    }

    return 0;
}

