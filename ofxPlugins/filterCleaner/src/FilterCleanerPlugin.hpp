#ifndef _TUTTLE_PLUGIN_FILTERCLEANER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_FILTERCLEANER_PLUGIN_HPP_

#include "FilterCleanerDefinitions.hpp"

#include <tuttle/plugin/ImageEffectGilPlugin.hpp>

namespace tuttle {
namespace plugin {
namespace filterCleaner {

template<typename Scalar>
struct FilterCleanerProcessParams
{
    EParamAlgo _algo;
    float fRedFilterColor;
    float fGreenFilterColor;
    float fBlueFilterColor;
    bool bInvert;
};

/**
 * @brief FilterCleaner plugin
 */
class FilterCleanerPlugin : public ImageEffectGilPlugin
{
public:
    typedef float Scalar;
public:
    FilterCleanerPlugin( OfxImageEffectHandle handle );

public:
    FilterCleanerProcessParams<Scalar> getProcessParams( const OfxPointD& renderScale = OFX::kNoRenderScale ) const;

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
