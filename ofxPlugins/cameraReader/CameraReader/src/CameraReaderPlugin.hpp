#ifndef _TUTTLE_PLUGIN_MLVREADER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_MLVREADER_PLUGIN_HPP_

#include "CameraReaderDefinitions.hpp"
#include "CameraVideo.hpp"

#include <tuttle/plugin/context/ReaderPlugin.hpp>

#include <boost/filesystem/path.hpp>

namespace tuttle {
namespace plugin {
namespace cameraReader {

struct CameraReaderProcessParams
{
    std::string _filepath;
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
    void ensureVideoOpened();

public:
    boost::filesystem::path _currentFilename;           ///< Current played file
    std::unique_ptr<tuttle::io::CameraVideo> _mlvVideo;    ///< Current file
    std::size_t _lastFrame;     ///< Last frame index
};

}
}
}

#endif
