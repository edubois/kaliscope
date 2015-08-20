#ifndef _TUTTLE_PLUGIN_DCRAWREADER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_DCRAWREADER_PLUGIN_HPP_

#include "DcrawReaderDefinitions.hpp"

#include <tuttle/plugin/context/ReaderPlugin.hpp>

#include <boost/filesystem/path.hpp>

namespace tuttle {
namespace plugin {
namespace dcrawReader {

struct DcrawReaderProcessParams
{
    boost::filesystem::path _filepath;
};

/**
 * @brief DcrawReader plugin
 */
class DcrawReaderPlugin : public ReaderPlugin
{
public:
    DcrawReaderPlugin( OfxImageEffectHandle handle );

public:
    DcrawReaderProcessParams getProcessParams( const OfxTime time ) const;
    void changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName );
    bool getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod );
    void getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences );
    void beginSequenceRender( const OFX::BeginSequenceRenderArguments& args );
    void render( const OFX::RenderArguments &args );

public:
    std::size_t _lastFrame;     ///< Last frame index
};

}
}
}

#endif
