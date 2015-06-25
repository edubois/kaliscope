#include "ColorMaskRemoverPlugin.hpp"
#include "ColorMaskRemoverProcess.hpp"
#include "ColorMaskRemoverDefinitions.hpp"

#include <boost/gil/gil_all.hpp>

namespace tuttle {
namespace plugin {
namespace colorMaskRemover {


ColorMaskRemoverPlugin::ColorMaskRemoverPlugin( OfxImageEffectHandle handle )
: ImageEffectGilPlugin( handle )
{
    _paramAlgo = fetchChoiceParam( kParamAlgorithm );
    _paramRedFilterColor = fetchIntParam( kParamRedFilterColor );
    _paramGreenFilterColor = fetchIntParam( kParamGreenFilterColor );
    _paramBlueFilterColor = fetchIntParam( kParamBlueFilterColor );
    _paramColorInvert = fetchBooleanParam( kParamColorInvert );
}

ColorMaskRemoverProcessParams<ColorMaskRemoverPlugin::Scalar> ColorMaskRemoverPlugin::getProcessParams( const OfxPointD& renderScale ) const
{
    ColorMaskRemoverProcessParams<Scalar> params;
    params._algo  = static_cast<EParamAlgo>( _paramAlgo->getValue() );
    params.fRedFilterColor = _paramRedFilterColor->getValue() / 255.0f;
    params.fGreenFilterColor = _paramGreenFilterColor->getValue() / 255.0f;
    params.fBlueFilterColor = _paramBlueFilterColor->getValue() / 255.0f;
    params.bInvert = _paramColorInvert->getValue();
    return params;
}

void ColorMaskRemoverPlugin::changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName )
{
    if( paramName == kParamHelpButton )
    {
        sendMessage( OFX::Message::eMessageMessage,
                     "This plugin provides a way to reduce orange mask that can appears on some processed films",
                     kParamHelpString );
    }
}

bool ColorMaskRemoverPlugin::isIdentity( const OFX::RenderArguments& args, OFX::Clip*& identityClip, double& identityTime )
{
    return false;
}

/**
 * @brief The overridden render function
 * @param[in]   args     Rendering parameters
 */
void ColorMaskRemoverPlugin::render( const OFX::RenderArguments &args )
{
    doGilRender<ColorMaskRemoverProcess>( *this, args );
}


}
}
}
