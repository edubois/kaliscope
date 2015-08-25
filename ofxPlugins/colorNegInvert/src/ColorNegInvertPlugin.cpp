#include "ColorNegInvertPlugin.hpp"
#include "ColorNegInvertProcess.hpp"
#include "ColorNegInvertAnalyzingProcess.hpp"
#include "ColorNegInvertDefinitions.hpp"

#include <boost/format.hpp>
#include <boost/gil/gil_all.hpp>

namespace tuttle {
namespace plugin {
namespace colorNegInvert {


ColorNegInvertPlugin::ColorNegInvertPlugin( OfxImageEffectHandle handle )
: ImageEffectGilPlugin( handle )
, _analyze( false )
, _redFilterColorToApply( 0.0 )
, _greenFilterColorToApply( 0.0 )
, _blueFilterColorToApply( 0.0 )
{
    _paramAlgo = fetchChoiceParam( kParamAlgorithm );
    _paramMaximumValue = fetchIntParam( kParamMaximumValue );

    _paramRedFilterColor = fetchDoubleParam( kParamRedFilterColor );
    _paramGreenFilterColor = fetchDoubleParam( kParamGreenFilterColor );
    _paramBlueFilterColor = fetchDoubleParam( kParamBlueFilterColor );

    _paramRedFactor = fetchDoubleParam( kParamRedFactor );
    _paramGreenFactor = fetchDoubleParam( kParamGreenFactor );
    _paramBlueFactor = fetchDoubleParam( kParamBlueFactor );

    _paramColorInvert = fetchBooleanParam( kParamColorInvert );
    _paramAnalyzeButton = fetchPushButtonParam( kParamAnalyzeButton );
    _paramForceNewRender = fetchIntParam( kParamFilterForceNewRender );

    _paramRedFilterColor->setRange( 0, _paramMaximumValue->getValue() );
    _paramRedFilterColor->setDisplayRange( 0, _paramMaximumValue->getValue() );
    _paramGreenFilterColor->setRange( 0, _paramMaximumValue->getValue() );
    _paramGreenFilterColor->setDisplayRange( 0, _paramMaximumValue->getValue() );
    _paramBlueFilterColor->setRange( 0, _paramMaximumValue->getValue() );
    _paramBlueFilterColor->setDisplayRange( 0, _paramMaximumValue->getValue() );
}

ColorNegInvertProcessParams<ColorNegInvertPlugin::Scalar> ColorNegInvertPlugin::getProcessParams( const OfxPointD& renderScale ) const
{
    ColorNegInvertProcessParams<Scalar> params;
    params._algo  = static_cast<EParamAlgo>( _paramAlgo->getValue() );
    const double maximumValue = _paramMaximumValue->getValue();
    params.fRedFilterColor = _paramRedFilterColor->getValue() / maximumValue;
    params.fGreenFilterColor = _paramGreenFilterColor->getValue() / maximumValue;
    params.fBlueFilterColor = _paramBlueFilterColor->getValue() / maximumValue;
    params.fRedFactor = _paramRedFactor->getValue() / 100.0f;
    params.fGreenFactor = _paramGreenFactor->getValue() / 100.0f;
    params.fBlueFactor = _paramBlueFactor->getValue() / 100.0f;
    params.bInvert = _paramColorInvert->getValue();
    return params;
}

void ColorNegInvertPlugin::changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName )
{
    if( paramName == kParamHelpButton )
    {
        sendMessage( OFX::Message::eMessageMessage, kParamHelpLabel, kParamHelpString );
    }
    else if ( paramName == kParamAnalyzeButton && _analyze == true )
    {
        _analyze = false;
        _paramRedFilterColor->setValue( _redFilterColorToApply );
        _paramGreenFilterColor->setValue( _greenFilterColorToApply );
        _paramBlueFilterColor->setValue( _blueFilterColorToApply );
        _paramAnalyzeButton->setLabels( kParamAnalyzeLabel, kParamAnalyzeLabel, kParamAnalyzeLabel );
        _paramAnalyzeButton->setHint( "Click one time to analyze the filter color" );
    }
    else if ( paramName == kParamAnalyzeButton && _analyze == false )
    {
        _analyze = true;
        _paramForceNewRender->setValue( !_paramForceNewRender->getValue() );
        _paramAnalyzeButton->setLabels( kParamApplyParameters, kParamApplyParameters, kParamApplyParameters );
        _paramAnalyzeButton->setHint( "Click another time to apply parameters" );
    }
    else if ( paramName == kParamMaximumValue )
    {
        _paramRedFilterColor->setRange( 0, _paramMaximumValue->getValue() );
        _paramRedFilterColor->setDisplayRange( 0, _paramMaximumValue->getValue() );
        _paramGreenFilterColor->setRange( 0, _paramMaximumValue->getValue() );
        _paramGreenFilterColor->setDisplayRange( 0, _paramMaximumValue->getValue() );
        _paramBlueFilterColor->setRange( 0, _paramMaximumValue->getValue() );
        _paramBlueFilterColor->setDisplayRange( 0, _paramMaximumValue->getValue() );
    }
}

/**
 * @brief update parameters with the filter color
 */    
void ColorNegInvertPlugin::notifyRGBFilterColor( const double r, const double g, const double b )
{
    const double vmax = _paramMaximumValue->getValue();
    _redFilterColorToApply = r * vmax;
    _greenFilterColorToApply = g * vmax;
    _blueFilterColorToApply = b * vmax;
}

bool ColorNegInvertPlugin::isIdentity( const OFX::RenderArguments& args, OFX::Clip*& identityClip, double& identityTime )
{
    return false;
}

/**
 * @brief The overridden render function
 * @param[in]   args     Rendering parameters
 */
void ColorNegInvertPlugin::render( const OFX::RenderArguments &args )
{
    if ( _analyze )
    {
        doGilRender<ColorNegInvertAnalyzingProcess>( *this, args );
    }
    else
    {
        doGilRender<ColorNegInvertProcess>( *this, args );
    }
}


}
}
}
