#ifndef _TUTTLE_PLUGIN_CAMERAREADER_PROCESS_HPP_
#define _TUTTLE_PLUGIN_CAMERAREADER_PROCESS_HPP_

#include "CameraInput.hpp"

#include <tuttle/plugin/ImageGilProcessor.hpp>

namespace tuttle {
namespace plugin {
namespace cameraReader {

/**
 * @brief CameraReader process
 *
 */
template<class View>
class CameraReaderProcess : public ImageGilProcessor<View>
{
public:
    typedef typename View::value_type Pixel;
    typedef typename boost::gil::channel_type<View>::type Channel;
    typedef float Scalar;
protected:
    CameraReaderPlugin&    _plugin;            ///< Rendering plugin
    CameraReaderProcessParams _params;         ///< parameters

public:
    CameraReaderProcess( CameraReaderPlugin& effect );

    void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );

private:
    // Read camera frame
    View& readFrame( View& dst );

    tuttle::io::CameraInput _cameraInput;  ///< Camera source
};

}
}
}

#include "CameraReaderProcess.tcc"

#endif
