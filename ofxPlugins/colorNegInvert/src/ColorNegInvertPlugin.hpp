#ifndef _TUTTLE_PLUGIN_COLORNEGINVERT_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_COLORNEGINVERT_PLUGIN_HPP_

#include "ColorNegInvertDefinitions.hpp"

#include <tuttle/plugin/ImageEffectGilPlugin.hpp>

namespace tuttle {
namespace plugin {
namespace colorNegInvert {

template<typename Scalar>
struct ColorNegInvertProcessParams
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
 * @brief ColorNegInvert plugin
 */
class ColorNegInvertPlugin : public ImageEffectGilPlugin
{
public:
    typedef float Scalar;
public:
    ColorNegInvertPlugin( OfxImageEffectHandle handle );

public:
    ColorNegInvertProcessParams<Scalar> getProcessParams( const OfxPointD& renderScale = OFX::kNoRenderScale ) const;

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
