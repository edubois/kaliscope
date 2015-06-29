#include "ColorMaskRemoverPluginFactory.hpp"
#include "ColorMaskRemoverPlugin.hpp"
#include "ColorMaskRemoverDefinitions.hpp"
#include "ofxsImageEffect.h"

#include <limits>

namespace tuttle {
namespace plugin {
namespace colorMaskRemover {

static const bool kSupportTiles = false;


/**
 * @brief Function called to describe the plugin main features.
 * @param[in, out] desc Effect descriptor
 */
void ColorMaskRemoverPluginFactory::describe( OFX::ImageEffectDescriptor& desc )
{
    desc.setLabels(
            "ColorMaskRemover",
            "ColorMaskRemover",
            "Remove mask (often orange mask) from an image" );
    desc.setPluginGrouping( "djarlabs" );

    desc.setDescription( "Remove colored mask from the image. An orange mask is often appearing on silver films, this helps removing it" );

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
void ColorMaskRemoverPluginFactory::describeInContext( OFX::ImageEffectDescriptor& desc,
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
    paramAlgo->setHint( "Colored mask removal algorithm" );
    paramAlgo->appendOption( kParamAlgorithmRGBReduction );
//    paramAlgo->appendOption( kParamAlgorithmYUVReduction );

    OFX::GroupParamDescriptor *groupFilterColorsParams = desc.defineGroupParam( "Mask color" );

    OFX::IntParamDescriptor *maximumValue = desc.defineIntParam( kParamMaximumValue );
    maximumValue->setLabels( kParamMaximumValueLabel, kParamMaximumValueLabel, kParamMaximumValueLabel );
    maximumValue->setParent( *groupFilterColorsParams );
    maximumValue->setDefault( kParamDefaultMaximumValue );
    maximumValue->setDisplayRange( 0, 4096 );
    maximumValue->setHint( "Maximum possible value of a channel value (usually 255 for 8 bit images)" );

    OFX::DoubleParamDescriptor *redFilterColor = desc.defineDoubleParam( kParamRedFilterColor );
    redFilterColor->setLabels( kParamRedFilterColorLabel, kParamRedFilterColorLabel, kParamRedFilterColorLabel );
    redFilterColor->setParent( *groupFilterColorsParams );
    redFilterColor->setDefault( kParamDefaultRedFilterColor );
    redFilterColor->setRange( 0, 255 );
    redFilterColor->setDisplayRange( 0, 255 );
    redFilterColor->setHint( "Red color of the filter" );

    OFX::DoubleParamDescriptor *greenFilterColor = desc.defineDoubleParam( kParamGreenFilterColor );
    greenFilterColor->setLabels( kParamGreenFilterColorLabel, kParamGreenFilterColorLabel, kParamGreenFilterColorLabel );
    greenFilterColor->setParent( *groupFilterColorsParams );
    greenFilterColor->setDefault( kParamDefaultGreenFilterColor );
    greenFilterColor->setRange( 0, 255 );
    greenFilterColor->setDisplayRange( 0, 255 );
    greenFilterColor->setHint( "Green color of the filter" );

    OFX::DoubleParamDescriptor *blueFilterColor = desc.defineDoubleParam( kParamBlueFilterColor );
    blueFilterColor->setLabels( kParamBlueFilterColorLabel, kParamBlueFilterColorLabel, kParamBlueFilterColorLabel );
    blueFilterColor->setParent( *groupFilterColorsParams );
    blueFilterColor->setDefault( kParamDefaultBlueFilterColor );
    blueFilterColor->setRange( 0, 255 );
    blueFilterColor->setDisplayRange( 0, 255 );
    blueFilterColor->setHint( "Blue color of the filter" );

    OFX::DoubleParamDescriptor *redFactor = desc.defineDoubleParam( kParamRedFactor );
    redFactor->setLabels( kParamRedFactorLabel, kParamRedFactorLabel, kParamRedFactorLabel );
    redFactor->setParent( *groupFilterColorsParams );
    redFactor->setDefault( kParamDefaultRedFactorColor );
    redFactor->setRange( 0, 300.0 );
    redFactor->setDisplayRange( 0, 300.0 );
    redFactor->setHint( "Red factor" );

    OFX::DoubleParamDescriptor *greenFactor = desc.defineDoubleParam( kParamGreenFactor );
    greenFactor->setLabels( kParamGreenFactorLabel, kParamGreenFactorLabel, kParamGreenFactorLabel );
    greenFactor->setParent( *groupFilterColorsParams );
    greenFactor->setDefault( kParamDefaultGreenFactorColor );
    greenFactor->setRange( 0, 300.0 );
    greenFactor->setDisplayRange( 0, 300.0 );
    greenFactor->setHint( OFX::getImageEffectHostDescription()->hostName );

    OFX::DoubleParamDescriptor *blueFactor = desc.defineDoubleParam( kParamBlueFactor );
    blueFactor->setLabels( kParamBlueFactorLabel, kParamBlueFactorLabel, kParamBlueFactorLabel );
    blueFactor->setParent( *groupFilterColorsParams );
    blueFactor->setDefault( kParamDefaultBlueFactorColor );
    blueFactor->setRange( 0, 300.0 );
    blueFactor->setDisplayRange( 0, 300.0 );
    blueFactor->setHint( "Blue factor" );

    OFX::BooleanParamDescriptor *colorInvert = desc.defineBooleanParam( kParamColorInvert );
    colorInvert->setLabels( kParamColorInvertLabel, kParamColorInvertLabel, kParamColorInvertLabel );
    colorInvert->setParent( *groupFilterColorsParams );
    colorInvert->setDefault( kParamDefaultColorInvertValue );

    OFX::PushButtonParamDescriptor* help = desc.definePushButtonParam( kParamHelpButton );
    help->setLabel( kParamHelpLabel );

    OFX::PushButtonParamDescriptor* analyze = desc.definePushButtonParam( kParamAnalyzeButton );
    analyze->setLabel( kParamAnalyzeLabel );
    analyze->setHint( kParamAnalyzeHint );

    OFX::IntParamDescriptor* forceNewRender = desc.defineIntParam( kParamFilterForceNewRender );
    forceNewRender->setLabel( "Force new render" );
    forceNewRender->setEnabled( false );
    forceNewRender->setIsSecret( true );
    forceNewRender->setIsPersistant( false );
    forceNewRender->setAnimates( false );
    forceNewRender->setCacheInvalidation( OFX::eCacheInvalidateValueAll );
    forceNewRender->setEvaluateOnChange( true );
    forceNewRender->setDefault( 0 );
}

/**
 * @brief Function called to create a plugin effect instance
 * @param[in] handle  Effect handle
 * @param[in] context Application context
 * @return  plugin instance
 */
OFX::ImageEffect* ColorMaskRemoverPluginFactory::createInstance( OfxImageEffectHandle handle,
                                                            OFX::EContext context )
{
    return new ColorMaskRemoverPlugin( handle );
}

}
}
}

