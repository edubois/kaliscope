#ifndef _KALISCOPE_PLUGIN_CAMERAREADER_PLUGIN_HPP_
#define _KALISCOPE_PLUGIN_CAMERAREADER_PLUGIN_HPP_

#include "CameraReaderDefinitions.hpp"

#include <boost/gil/channel_algorithm.hpp> // force to use the boostHack version first
#include <boost/filesystem/path.hpp>

#include <tuttle/plugin/ImageEffectGilPlugin.hpp>
#include <tuttle/plugin/context/ReaderDefinition.hpp>
#include <tuttle/plugin/exceptions.hpp>

namespace tuttle {
namespace plugin {
namespace cameraReader {

/**
 * @brief CameraReader plugin
 */
class CameraReaderPlugin : public OFX::ImageEffect
{
public:
	CameraReaderPlugin( OfxImageEffectHandle handle );
	virtual ~CameraReaderPlugin() = 0;

public:
	virtual void changedParam( const OFX::InstanceChangedArgs& args, const std::string& paramName );
	virtual bool getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod ) = 0;
	virtual void getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences );
	virtual bool getTimeDomain( OfxRangeD& range );
	virtual void render( const OFX::RenderArguments& args );

public:
	OfxTime getFirstTime() const
	{
            return 0.0;
	}

	OfxTime getLastTime() const
	{
            return kOfxFlagInfiniteMax;
	}

	EParamReaderBitDepth getExplicitBitDepthConversion() const
	{
            return static_cast<EParamReaderBitDepth>( _paramBitDepth->getValue() );
	}

	
	EParamReaderChannel getExplicitChannelConversion() const
	{
            return static_cast<EParamReaderChannel>( _paramChannel->getValue() );
	}
	
	OFX::EBitDepth getOfxExplicitConversion() const
	{
            switch( getExplicitBitDepthConversion() )
            {
                case eParamReaderBitDepthByte:
                        return OFX::eBitDepthUByte;
                case eParamReaderBitDepthShort:
                        return OFX::eBitDepthUShort;
                case eParamReaderBitDepthFloat:
                        return OFX::eBitDepthFloat;
                case eParamReaderBitDepthAuto:
                        BOOST_THROW_EXCEPTION( exception::Value() );
            }
            return OFX::eBitDepthNone;
	}

protected:
	OFX::Clip*           _clipDst;        ///< Destination image clip
	/// @name user parameters
	/// @{
	OFX::ChoiceParam*    _paramCamera;          ///< Camera
	OFX::ChoiceParam*    _paramBitDepth;        ///< Explicit bit depth conversion
	OFX::ChoiceParam*    _paramChannel;         ///< Explicit component conversion
	OFX::ChoiceParam*    _paramResolution;      ///< Resolution
	OFX::ChoiceParam*    _paramFocusMode;       ///< Focus mode
	OFX::ChoiceParam*    _paramAperture;        ///< Camera aperture
	OFX::ChoiceParam*    _paramShutterSpeed;    ///< Camera shutter speed
	OFX::ChoiceParam*    _paramISOSensitivity;      ///< Camera iso speed
	OFX::DoubleParam*    _paramExposureCompensation;    ///< Exposure compensation factor
	OFX::BooleanParam*   _paramAutoExposure;    ///< Automatic exposure
	OFX::BooleanParam*   _paramAutoISO;         ///< Automatic sensibility
	OFX::BooleanParam*   _paramAutoShutterSpeed;///< Automatic shutter speed
	OFX::BooleanParam*   _paramAutoAperture;    ///< Automatic aperture
	OFX::DoubleParam*    _paramOpticalZoomFactor;     ///< Optical zoom factor
	OFX::DoubleParam*    _paramWhiteBalance;    ///< Image white balance in Kelvin
	/// @}
};

}
}
}

#endif
