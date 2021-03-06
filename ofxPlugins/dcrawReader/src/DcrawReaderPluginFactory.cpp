/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

#include "DcrawReaderPluginFactory.hpp"
#include "DcrawReaderPlugin.hpp"
#include "DcrawReaderDefinitions.hpp"
#include "ofxsImageEffect.h"

#include <tuttle/plugin/context/ReaderPluginFactory.hpp>

#include <limits>

namespace tuttle {
namespace plugin {
namespace dcrawReader {

static const bool kSupportTiles = false;


/**
 * @brief Function called to describe the plugin main features.
 * @param[in, out] desc Effect descriptor
 */
void DcrawReaderPluginFactory::describe( OFX::ImageEffectDescriptor& desc )
{
	desc.setLabels(
		"DcrawReader",
		"DcrawReader",
		"DCRAW reader" );
        desc.setPluginGrouping( "djarlabs" );

	// add the supported contexts, only filter at the moment
	desc.addSupportedContext( OFX::eContextReader );
	desc.addSupportedContext( OFX::eContextGeneral );

	// add supported pixel depths
	desc.addSupportedBitDepth( OFX::eBitDepthFloat );

	// plugin flags
	desc.setRenderThreadSafety( OFX::eRenderInstanceSafe );
	desc.setHostFrameThreading( false );
	desc.setSupportsMultiResolution( false );
	desc.setSupportsMultipleClipDepths( true );
	desc.setSupportsMultipleClipPARs( true );
	desc.setSupportsTiles( kSupportTiles );
}

/**
 * @brief Function called to describe the plugin controls and features.
 * @param[in, out]   desc       Effect descriptor
 * @param[in]        context    Application context
 */
void DcrawReaderPluginFactory::describeInContext( OFX::ImageEffectDescriptor& desc,
                                                  OFX::EContext context )
{
    // Create the mandated output clip
    OFX::ClipDescriptor* dstClip = desc.defineClip( kOfxImageEffectOutputClipName );
    dstClip->addSupportedComponent( OFX::ePixelComponentRGB );
    dstClip->setSupportsTiles( kSupportTiles );

    OFX::ChoiceParamDescriptor* paramInterpolationQuality = desc.defineChoiceParam( kParamInterpolationQuality );
    paramInterpolationQuality->setLabel( "Interpolation quality" );
    paramInterpolationQuality->setHint( "DCRaw interpolation quality" );
    paramInterpolationQuality->appendOption( kParamInterpolationQualityLinear );
    paramInterpolationQuality->appendOption( kParamInterpolationQualityVNG );
    paramInterpolationQuality->appendOption( kParamInterpolationQualityPPG );
    paramInterpolationQuality->appendOption( kParamAlgorithmYUVReductionAHD );
    paramInterpolationQuality->setDefault( 3 );

    describeReaderParamsInContext( desc, context );
}

/**
 * @brief Function called to create a plugin effect instance
 * @param[in] handle  Effect handle
 * @param[in] context Application context
 * @return  plugin instance
 */
OFX::ImageEffect* DcrawReaderPluginFactory::createInstance( OfxImageEffectHandle handle,
                                                          OFX::EContext context )
{
    return new DcrawReaderPlugin( handle );
}

}
}
}

