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
private:
    OFX::ChoiceParam*	_paramAlgo;
    OFX::IntParam*	_paramRedFilterColor;
    OFX::IntParam*	_paramGreenFilterColor;
    OFX::IntParam*	_paramBlueFilterColor;
    OFX::BooleanParam*	_paramColorInvert;
};

}
}
}

#endif
