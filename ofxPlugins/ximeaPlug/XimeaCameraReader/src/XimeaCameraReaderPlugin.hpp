#ifndef _TUTTLE_PLUGIN_XIMEACAMERAREADER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_XIMEACAMERAREADER_PLUGIN_HPP_

#include "XimeaCameraReaderDefinitions.hpp"

#include <tuttle/plugin/ImageEffectGilPlugin.hpp>

namespace tuttle {
namespace plugin {
namespace ximeaCameraReader {

template<typename Scalar>
struct XimeaCameraReaderProcessParams
{
	
};

/**
 * @brief XimeaCameraReader plugin
 */
class XimeaCameraReaderPlugin : public ImageEffectGilPlugin
{
public:
	typedef float Scalar;
public:
    XimeaCameraReaderPlugin( OfxImageEffectHandle handle );

public:
	XimeaCameraReaderProcessParams<Scalar> getProcessParams( const OfxPointD& renderScale = OFX::kNoRenderScale ) const;

    void changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName );

//	bool getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod );
//	void getRegionsOfInterest( const OFX::RegionsOfInterestArguments& args, OFX::RegionOfInterestSetter& rois );
	bool isIdentity( const OFX::RenderArguments& args, OFX::Clip*& identityClip, double& identityTime );

    void render( const OFX::RenderArguments &args );
	
public:
//    OFX::Clip* _clipSrcMatte; ///< Matte source image clip
};

}
}
}

#endif
