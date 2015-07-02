#include "CameraReaderPlugin.hpp"
#include "CameraReaderProcess.hpp"
#include "CameraReaderDefinitions.hpp"
#include "CameraInput.hpp"


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
    return params;
}

void CameraReaderPlugin::changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName )
{
    ReaderPlugin::changedParam( args, paramName );
}

void CameraReaderPlugin::ensureInputOpened()
{
    if ( !_cameraInput )
    {
        _cameraInput.reset( new tuttle::io::CameraInput() );
    }
}

bool CameraReaderPlugin::getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod )
{
    ensureInputOpened();

    rod.x1 = 0;
    rod.x2 = _cameraInput->width() * this->_clipDst->getPixelAspectRatio();
    rod.y1 = 0;
    rod.y2 = _cameraInput->height();
    return true;
}

void CameraReaderPlugin::getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences )
{
    ReaderPlugin::getClipPreferences( clipPreferences );

    ensureInputOpened();

    clipPreferences.setClipBitDepth( *_clipDst, OFX::eBitDepthUByte );
    clipPreferences.setClipComponents( *_clipDst, OFX::ePixelComponentRGB );
/*
    if( getExplicitConversion() == eParamReaderExplicitConversionAuto )
    {
        OFX::EBitDepth bd = OFX::eBitDepthNone;
        switch( cameraInput.componentsType() )
        {
            case CameraInput::eCompTypeR8G8B8:
            case CameraInput::eCompTypeR8G8B8A8:
            case CameraInput::eCompTypeA8B8G8R8:
            {
                bd = OFX::eBitDepthUByte;
                break;
            }
            case CameraInput::eCompTypeR10G10B10:
            case CameraInput::eCompTypeR10G10B10A10:
            case CameraInput::eCompTypeA10B10G10R10:
            case CameraInput::eCompTypeR12G12B12:
            case CameraInput::eCompTypeR12G12B12A12:
            case CameraInput::eCompTypeA12B12G12R12:
            case CameraInput::eCompTypeR16G16B16:
            case CameraInput::eCompTypeR16G16B16A16:
            case CameraInput::eCompTypeA16B16G16R16:
            {
                bd = OFX::eBitDepthUShort;
                break;
            }
            default:
                bd = OFX::eBitDepthFloat;
        }

        clipPreferences.setClipBitDepth( *_clipDst, bd );
    }
    switch( cameraInput.components() )
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
    ensureInputOpened();

    range.min = std::numeric_limits<double>::min();
    range.max = std::numeric_limits<double>::max();

    return true;
}

void CameraReaderPlugin::beginSequenceRender( const OFX::BeginSequenceRenderArguments& args )
{
    ReaderPlugin::beginSequenceRender( args );

    ensureInputOpened();
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
