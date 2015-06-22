#include "FilterCleanerPlugin.hpp"
#include "FilterCleanerProcess.hpp"
#include "FilterCleanerDefinitions.hpp"

#include <boost/gil/gil_all.hpp>

namespace tuttle {
namespace plugin {
namespace filterCleaner {


FilterCleanerPlugin::FilterCleanerPlugin( OfxImageEffectHandle handle )
: ImageEffectGilPlugin( handle )
{
    _paramAlgo = fetchChoiceParam( kParamAlgorithm );
    _paramRedFilterColor = fetchIntParam( kParamRedFilterColor );
    _paramGreenFilterColor = fetchIntParam( kParamGreenFilterColor );
    _paramBlueFilterColor = fetchIntParam( kParamBlueFilterColor );
}

FilterCleanerProcessParams<FilterCleanerPlugin::Scalar> FilterCleanerPlugin::getProcessParams( const OfxPointD& renderScale ) const
{
    FilterCleanerProcessParams<Scalar> params;
    params._algo  = static_cast<EParamAlgo>( _paramAlgo->getValue() );
    params.fRedFilterColor = _paramRedFilterColor->getValue() / 255.0f;
    params.fGreenFilterColor = _paramGreenFilterColor->getValue() / 255.0f;
    params.fBlueFilterColor = _paramBlueFilterColor->getValue() / 255.0f;
    return params;
}

void FilterCleanerPlugin::changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName )
{
    if( paramName == kParamHelpButton )
    {
        sendMessage( OFX::Message::eMessageMessage,
                     "Go on a frame supposed to be white, click on analyze, then the filter should be removed.",
                     kParamHelpString );
    }
}

bool FilterCleanerPlugin::isIdentity( const OFX::RenderArguments& args, OFX::Clip*& identityClip, double& identityTime )
{
    return false;
}

/**
 * @brief The overridden render function
 * @param[in]   args     Rendering parameters
 */
void FilterCleanerPlugin::render( const OFX::RenderArguments &args )
{
    doGilRender<FilterCleanerProcess>( *this, args );
}


}
}
}