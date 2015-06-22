#include "FilterCleanerPluginFactory.hpp"
#include "FilterCleanerPlugin.hpp"
#include "FilterCleanerDefinitions.hpp"
#include "ofxsImageEffect.h"

#include <limits>

namespace tuttle {
namespace plugin {
namespace filterCleaner {

static const bool kSupportTiles = false;


/**
 * @brief Function called to describe the plugin main features.
 * @param[in, out] desc Effect descriptor
 */
void FilterCleanerPluginFactory::describe( OFX::ImageEffectDescriptor& desc )
{
    desc.setLabels(
            "FilterCleaner",
            "FilterCleaner",
            "Remove filter (often orange filter) from image" );
    desc.setPluginGrouping( "tuttle" );

    desc.setDescription( "Remove filter from the image. An orange filter is often appearing on silver films, this helps removing it." );

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
void FilterCleanerPluginFactory::describeInContext( OFX::ImageEffectDescriptor& desc,
                                                  OFX::EContext context )
{
    OFX::ClipDescriptor* srcClip = desc.defineClip( kOfxImageEffectSimpleSourceClipName );
    srcClip->addSupportedComponent( OFX::ePixelComponentRGB );
    srcClip->addSupportedComponent( OFX::ePixelComponentRGBA );
    srcClip->setSupportsTiles( kSupportTiles );

    // Create the mandated output clip
    OFX::ClipDescriptor* dstClip = desc.defineClip( kOfxImageEffectOutputClipName );
    dstClip->addSupportedComponent( OFX::ePixelComponentRGB );
    dstClip->addSupportedComponent( OFX::ePixelComponentRGBA );
    dstClip->setSupportsTiles( kSupportTiles );

    OFX::ChoiceParamDescriptor* paramAlgo = desc.defineChoiceParam( kParamAlgorithm );
    paramAlgo->setLabel( "Algorithm" );
    paramAlgo->setHint( "Filter removal algorithm" );
    paramAlgo->appendOption( kParamAlgorithmRGBReduction );
    paramAlgo->appendOption( kParamAlgorithmYUVReduction );

    OFX::GroupParamDescriptor *groupFilterColorsParams = desc.defineGroupParam( "Filter color" );

    OFX::IntParamDescriptor *redFilterColor = desc.defineIntParam( kParamRedFilterColor );
    redFilterColor->setLabels( kParamRedFilterColorLabel, kParamRedFilterColorLabel, kParamRedFilterColorLabel );
    redFilterColor->setParent( *groupFilterColorsParams );
    redFilterColor->setDefault( kParamDefaultRedFilterColor );
    redFilterColor->setRange( 0, 255 );
    redFilterColor->setDisplayRange( 0, 255 );
    redFilterColor->setHint( "Red color of the filter" );

    OFX::IntParamDescriptor *greenFilterColor = desc.defineIntParam( kParamGreenFilterColor );
    greenFilterColor->setLabels( kParamGreenFilterColorLabel, kParamGreenFilterColorLabel, kParamGreenFilterColorLabel );
    greenFilterColor->setParent( *groupFilterColorsParams );
    greenFilterColor->setDefault( kParamDefaultGreenFilterColor );
    greenFilterColor->setRange( 0, 255 );
    greenFilterColor->setDisplayRange( 0, 255 );
    greenFilterColor->setHint( "Green color of the filter" );

    OFX::IntParamDescriptor *blueFilterColor = desc.defineIntParam( kParamBlueFilterColor );
    blueFilterColor->setLabels( kParamBlueFilterColorLabel, kParamBlueFilterColorLabel, kParamBlueFilterColorLabel );
    blueFilterColor->setParent( *groupFilterColorsParams );
    blueFilterColor->setDefault( kParamDefaultBlueFilterColor );
    blueFilterColor->setRange( 0, 255 );
    blueFilterColor->setDisplayRange( 0, 255 );
    blueFilterColor->setHint( "Blue color of the filter" );

    OFX::PushButtonParamDescriptor* analyze = desc.definePushButtonParam( kParamAnalyze );
    analyze->setLabel( kParamAnalyzeLabel );

    OFX::PushButtonParamDescriptor* help = desc.definePushButtonParam( kParamHelpButton );
    help->setLabel( kParamHelpLabel );
}

/**
 * @brief Function called to create a plugin effect instance
 * @param[in] handle  Effect handle
 * @param[in] context Application context
 * @return  plugin instance
 */
OFX::ImageEffect* FilterCleanerPluginFactory::createInstance( OfxImageEffectHandle handle,
                                                            OFX::EContext context )
{
    return new FilterCleanerPlugin( handle );
}

}
}
}

