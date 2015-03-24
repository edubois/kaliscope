#include "XimeaCameraReaderPluginFactory.hpp"
#include "XimeaCameraReaderPlugin.hpp"
#include "XimeaCameraReaderDefinitions.hpp"
#include "ofxsImageEffect.h"

#include <limits>

namespace tuttle {
namespace plugin {
namespace ximeaCameraReader {

static const bool kSupportTiles = false;


/**
 * @brief Function called to describe the plugin main features.
 * @param[in, out] desc Effect descriptor
 */
void XimeaCameraReaderPluginFactory::describe( OFX::ImageEffectDescriptor& desc )
{
	desc.setLabels(
		"XimeaCameraReader",
		"XimeaCameraReader",
		"Ximea camera reader" );
	desc.setPluginGrouping( "tuttle" );

	desc.setDescription( "Plugin under early development." );

	// add the supported contexts, only filter at the moment
	desc.addSupportedContext( OFX::eContextFilter );
	desc.addSupportedContext( OFX::eContextGeneral );

	// add supported pixel depths
	desc.addSupportedBitDepth( OFX::eBitDepthUByte );
	desc.addSupportedBitDepth( OFX::eBitDepthUShort );
	desc.addSupportedBitDepth( OFX::eBitDepthFloat );

	// plugin flags
	desc.setSupportsTiles( kSupportTiles );
	desc.setRenderThreadSafety( OFX::eRenderFullySafe );
}

/**
 * @brief Function called to describe the plugin controls and features.
 * @param[in, out]   desc       Effect descriptor
 * @param[in]        context    Application context
 */
void XimeaCameraReaderPluginFactory::describeInContext( OFX::ImageEffectDescriptor& desc,
                                                  OFX::EContext context )
{
	OFX::ClipDescriptor* srcClip = desc.defineClip( kOfxImageEffectSimpleSourceClipName );
	srcClip->addSupportedComponent( OFX::ePixelComponentRGBA );
	srcClip->addSupportedComponent( OFX::ePixelComponentRGB );
	srcClip->addSupportedComponent( OFX::ePixelComponentAlpha );
	srcClip->setSupportsTiles( kSupportTiles );

	// Create the mandated output clip
	OFX::ClipDescriptor* dstClip = desc.defineClip( kOfxImageEffectOutputClipName );
	dstClip->addSupportedComponent( OFX::ePixelComponentRGBA );
	dstClip->addSupportedComponent( OFX::ePixelComponentRGB );
	dstClip->addSupportedComponent( OFX::ePixelComponentAlpha );
	dstClip->setSupportsTiles( kSupportTiles );
}

/**
 * @brief Function called to create a plugin effect instance
 * @param[in] handle  Effect handle
 * @param[in] context Application context
 * @return  plugin instance
 */
OFX::ImageEffect* XimeaCameraReaderPluginFactory::createInstance( OfxImageEffectHandle handle,
                                                            OFX::EContext context )
{
	return new XimeaCameraReaderPlugin( handle );
}

}
}
}

