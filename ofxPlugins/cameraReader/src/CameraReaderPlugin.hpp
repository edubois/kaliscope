#ifndef _TUTTLE_PLUGIN_CAMERAREADER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_CAMERAREADER_PLUGIN_HPP_

#include "CameraReaderDefinitions.hpp"
#include "CameraInput.hpp"

#include <tuttle/plugin/context/ReaderPlugin.hpp>

#include <boost/filesystem/path.hpp>

namespace tuttle {
namespace plugin {
namespace cameraReader {

struct CameraReaderProcessParams
{
};

/**
 * @brief CameraReader plugin
 */
class CameraReaderPlugin : public ReaderPlugin
{
public:
    CameraReaderPlugin( OfxImageEffectHandle handle );

public:
    CameraReaderProcessParams getProcessParams( const OfxTime time ) const;
    void changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName );
    bool getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod );
    bool getTimeDomain( OfxRangeD& range );
    void getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences );
    void beginSequenceRender( const OFX::BeginSequenceRenderArguments& args );
    void render( const OFX::RenderArguments &args );

private:
    void ensureInputOpened();

public:
    std::unique_ptr<tuttle::io::CameraInput> _cameraInput;    ///< Current file
    std::size_t _lastFrame;     ///< Last frame index
};

}
}
}

#endif
