#ifndef _TUTTLE_PLUGIN_FILTERCLEANER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_FILTERCLEANER_PLUGIN_HPP_

#include "ColorMaskRemoverDefinitions.hpp"

#include <tuttle/plugin/ImageEffectGilPlugin.hpp>

namespace tuttle {
namespace plugin {
namespace colorMaskRemover {

template<typename Scalar>
struct ColorMaskRemoverProcessParams
{
    EParamAlgo _algo;
    float fRedFilterColor;
    float fGreenFilterColor;
    float fBlueFilterColor;
    float fRedFactor;
    float fGreenFactor;
    float fBlueFactor;
    bool bInvert;
};

/**
 * @brief ColorMaskRemover plugin
 */
class ColorMaskRemoverPlugin : public ImageEffectGilPlugin
{
public:
    typedef float Scalar;
public:
    ColorMaskRemoverPlugin( OfxImageEffectHandle handle );

public:
    ColorMaskRemoverProcessParams<Scalar> getProcessParams( const OfxPointD& renderScale = OFX::kNoRenderScale ) const;

    void changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName );

    bool isIdentity( const OFX::RenderArguments& args, OFX::Clip*& identityClip, double& identityTime );

    void render( const OFX::RenderArguments &args );

    /**
     * @brief display/update the filter color
     */    
    void notifyRGBFilterColor( const double r, const double g, const double b );
private:
    bool _analyze;              ///< Analyze color of the mask (set this on an image supposed to be white)
    double _redFilterColorToApply;
    double _greenFilterColorToApply;
    double _blueFilterColorToApply;

    OFX::InstanceChangedArgs _instanceChangedArgs;
    OFX::ChoiceParam*	_paramAlgo;
    OFX::PushButtonParam*	_paramAnalyzeButton;
    OFX::IntParam*	_paramMaximumValue;
    OFX::IntParam*	_paramForceNewRender;
    OFX::DoubleParam*	_paramRedFilterColor;
    OFX::DoubleParam*	_paramGreenFilterColor;
    OFX::DoubleParam*	_paramBlueFilterColor;
    OFX::DoubleParam*	_paramRedFactor;
    OFX::DoubleParam*	_paramGreenFactor;
    OFX::DoubleParam*	_paramBlueFactor;
    OFX::BooleanParam*	_paramColorInvert;
};

}
}
}

#endif
