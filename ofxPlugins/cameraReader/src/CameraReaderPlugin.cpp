#include "CameraReaderPlugin.hpp"

namespace tuttle {
namespace plugin {
namespace cameraReader {

CameraReaderPlugin::CameraReaderPlugin( OfxImageEffectHandle handle )
	: OFX::ImageEffect( handle )
{
	_clipDst       = fetchClip( kOfxImageEffectOutputClipName );
	_paramCamera   = fetchChoiceParam( kParamCameraChoice );

	_paramResolution = fetchChoiceParam( kParamResolutionChoice );

	_paramFocusMode = fetchChoiceParam( kParamFocusMode );
	_paramAperture = fetchChoiceParam( kParamApertureChoice );
	_paramShutterSpeed = fetchChoiceParam( kParamShutterSpeedChoice );
	_paramISOSensitivity = fetchChoiceParam( kParamISOChoice );
	_paramExposureCompensation = fetchDoubleParam( kParamExposureCompensation );
	_paramAutoExposure = fetchBooleanParam( kParamAutoExposure );
	_paramAutoISO = fetchBooleanParam( kParamAutoSensibility );
	_paramAutoShutterSpeed = fetchBooleanParam( kParamAutoShutterSpeed );
	_paramAutoAperture = fetchBooleanParam( kParamAutoAperture );
	_paramOpticalZoomFactor = fetchDoubleParam( kParamOpticalZoomFactor );
	_paramWhiteBalance = fetchDoubleParam( kParamWhiteBalance );
        _paramWhiteBalance->setHint( "White balance in Kelvin. Set to 0 for automatic white balance" );

	_paramBitDepth = fetchChoiceParam( kTuttlePluginBitDepth );
	_paramChannel  = fetchChoiceParam( kTuttlePluginChannel );
}

CameraReaderPlugin::~CameraReaderPlugin()
{}

void CameraReaderPlugin::changedParam( const OFX::InstanceChangedArgs& args, const std::string& paramName )
{
}

void CameraReaderPlugin::getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences )
{
    clipPreferences.setOutputFrameVarying( true );

    switch( getExplicitBitDepthConversion() )
    {
        case eParamReaderBitDepthByte:
        {
            clipPreferences.setClipBitDepth( *this->_clipDst, OFX::eBitDepthUByte );
            break;
        }
        case eParamReaderBitDepthShort:
        {
            clipPreferences.setClipBitDepth( *this->_clipDst, OFX::eBitDepthUShort );
            break;
        }
        case eParamReaderBitDepthAuto:
        case eParamReaderBitDepthFloat:
        {
            clipPreferences.setClipBitDepth( *this->_clipDst, OFX::eBitDepthFloat );
            break;
        }
    }
    switch( getExplicitChannelConversion() )
    {
        case eParamReaderChannelGray:
        {
            clipPreferences.setClipComponents( *this->_clipDst, OFX::ePixelComponentAlpha );
            break;
        }
        case eParamReaderChannelRGB:
        {
            if( OFX::getImageEffectHostDescription()->supportsPixelComponent( OFX::ePixelComponentRGB ) )
                clipPreferences.setClipComponents( *this->_clipDst, OFX::ePixelComponentRGB );
            else
                clipPreferences.setClipComponents( *this->_clipDst, OFX::ePixelComponentRGBA );
            break;
        }
        case eParamReaderChannelAuto:
        case eParamReaderChannelRGBA:
        {
            clipPreferences.setClipComponents( *this->_clipDst, OFX::ePixelComponentRGBA );
            break;
        }
    }

    clipPreferences.setPixelAspectRatio( *this->_clipDst, 1.0 );
}

bool CameraReaderPlugin::getTimeDomain( OfxRangeD& range )
{
    range.min = getFirstTime();
    range.max = getLastTime();
    TUTTLE_TLOG( TUTTLE_INFO, "[CameraReaderPlugin plugin] Time Domain : " << range.min << " to " << range.max );
    return true;
}

void CameraReaderPlugin::render( const OFX::RenderArguments& args )
{
}

}
}
}
