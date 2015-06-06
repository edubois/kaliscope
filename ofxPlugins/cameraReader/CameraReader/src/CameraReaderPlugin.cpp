#include "CameraReaderPlugin.hpp"
#include "CameraReaderProcess.hpp"
#include "CameraReaderDefinitions.hpp"
#include "CameraVideo.hpp"


#include <boost/gil/gil_all.hpp>

namespace tuttle {
namespace plugin {
namespace cameraReader {


CameraReaderPlugin::CameraReaderPlugin( OfxImageEffectHandle handle )
: ReaderPlugin( handle )
{
}

CameraReaderProcessParams CameraReaderPlugin::getProcessParams( const OfxTime time ) const
{
    CameraReaderProcessParams params;
    params._filepath = getAbsoluteFilenameAt( time );
    return params;
}

void CameraReaderPlugin::changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName )
{
    ReaderPlugin::changedParam( args, paramName );
}

void CameraReaderPlugin::ensureVideoOpened()
{
    const std::string& filepath = _paramFilepath->getValue();
    if ( !_mlvVideo || _mlvVideo->filename() != _currentFilename )
    {
        _mlvVideo.reset( new tuttle::io::CameraVideo() );
        _currentFilename = filepath;
        _mlvVideo->readHeader( _currentFilename );
    }
}

bool CameraReaderPlugin::getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod )
{
    ensureVideoOpened();

    _mlvVideo->readHeader( getAbsoluteFilenameAt( args.time ) );
    rod.x1 = 0;
    rod.x2 = _mlvVideo->width() * this->_clipDst->getPixelAspectRatio();
    rod.y1 = 0;
    rod.y2 = _mlvVideo->height();
    return true;
}

void CameraReaderPlugin::getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences )
{
    ReaderPlugin::getClipPreferences( clipPreferences );

    ensureVideoOpened();

    clipPreferences.setClipBitDepth( *_clipDst, OFX::eBitDepthUByte );
    clipPreferences.setClipComponents( *_clipDst, OFX::ePixelComponentRGB );
/*
    if( getExplicitConversion() == eParamReaderExplicitConversionAuto )
    {
        OFX::EBitDepth bd = OFX::eBitDepthNone;
        switch( mlvVideo.componentsType() )
        {
            case CameraVideo::eCompTypeR8G8B8:
            case CameraVideo::eCompTypeR8G8B8A8:
            case CameraVideo::eCompTypeA8B8G8R8:
            {
                bd = OFX::eBitDepthUByte;
                break;
            }
            case CameraVideo::eCompTypeR10G10B10:
            case CameraVideo::eCompTypeR10G10B10A10:
            case CameraVideo::eCompTypeA10B10G10R10:
            case CameraVideo::eCompTypeR12G12B12:
            case CameraVideo::eCompTypeR12G12B12A12:
            case CameraVideo::eCompTypeA12B12G12R12:
            case CameraVideo::eCompTypeR16G16B16:
            case CameraVideo::eCompTypeR16G16B16A16:
            case CameraVideo::eCompTypeA16B16G16R16:
            {
                bd = OFX::eBitDepthUShort;
                break;
            }
            default:
                bd = OFX::eBitDepthFloat;
        }

        clipPreferences.setClipBitDepth( *_clipDst, bd );
    }
    switch( mlvVideo.components() )
    {
        case 3:
        {
            clipPreferences.setClipComponents( *this->_clipDst, OFX::ePixelComponentRGB );
            break;
        }

        case 4:
        {
            clipPreferences.setClipComponents( *this->_clipDst, OFX::ePixelComponentRGBA );
            break;
        }
        default:
        {
            clipPreferences.setClipComponents( *this->_clipDst, OFX::ePixelComponentRGBA );
            break;
        }
    }
*/
    clipPreferences.setPixelAspectRatio( *this->_clipDst, 1.0 );
}

bool CameraReaderPlugin::getTimeDomain( OfxRangeD& range )
{
    ensureVideoOpened();

    size_t nbFrames = _mlvVideo->frameCount();

    // if nbFrames is unknown
    if( nbFrames == 0 )
        nbFrames = 1;

    range.min = 0.0;
    range.max = nbFrames - 1.0;

    return true;
}

void CameraReaderPlugin::beginSequenceRender( const OFX::BeginSequenceRenderArguments& args )
{
    ReaderPlugin::beginSequenceRender( args );

    ensureVideoOpened();
}

/**
 * @brief The overridden render function
 * @param[in]   args     Rendering parameters
 */
void CameraReaderPlugin::render( const OFX::RenderArguments& args )
{
    ReaderPlugin::render( args );

    // instantiate the render code based on the pixel depth of the dst clip
    OFX::EBitDepth bitDepth         = _clipDst->getPixelDepth();
    OFX::EPixelComponent components = _clipDst->getPixelComponents();

    switch( components )
    {
        case OFX::ePixelComponentRGBA:
        {
            doGilRender<CameraReaderProcess, false, terry::rgba_layout_t>( *this, args, bitDepth );
            return;
        }
        case OFX::ePixelComponentRGB:
        {
            doGilRender<CameraReaderProcess, false, terry::rgb_layout_t>( *this, args, bitDepth );
            return;
        }
        case OFX::ePixelComponentAlpha:
        case OFX::ePixelComponentCustom:
        case OFX::ePixelComponentNone:
        {
            BOOST_THROW_EXCEPTION( exception::Unsupported()
                    << exception::user() + "Pixel components (" + mapPixelComponentEnumToString(components) + ") not supported by the plugin." );
        }
    }
    BOOST_THROW_EXCEPTION( exception::Unknown() );
}


}
}
}
