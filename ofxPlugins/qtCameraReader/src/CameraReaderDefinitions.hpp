#ifndef _TUTTLE_PLUGIN_CAMERAREADER_DEFINITIONS_HPP_
#define _TUTTLE_PLUGIN_CAMERAREADER_DEFINITIONS_HPP_

#include <tuttle/plugin/global.hpp>


namespace tuttle {
namespace plugin {
namespace cameraReader {

static const std::string kParamCameraChoice( "Camera" );
static const std::string kParamResolutionChoice( "Capture resolution" );
static const std::string kParamFocusMode( "Focus mode" );
static const std::string kParamApertureChoice( "Aperture" );
static const std::string kParamShutterSpeedChoice( "Shutter speed" );
static const std::string kParamISOChoice( "Sensibility (ISO)" );
static const std::string kParamExposureCompensation( "Exposure compensation" );
static const std::string kParamAutoExposure( "Automatic exposure" );
static const std::string kParamAutoSensibility( "Automatic sensibility" );
static const std::string kParamAutoShutterSpeed( "Automatic shutter speed" );
static const std::string kParamAutoAperture( "Automatic aperture" );
static const std::string kParamOpticalZoomFactor( "Optical zoom factor" );
static const std::string kParamWhiteBalance( "White balance (in Kelvin)" );

}
}
}

#endif
