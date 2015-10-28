/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

#ifndef _TUTTLE_PLUGIN_CAMERAREADER_PROCESS_HPP_
#define _TUTTLE_PLUGIN_CAMERAREADER_PROCESS_HPP_

#include <tuttle/plugin/ImageGilProcessor.hpp>

namespace tuttle {
namespace plugin {
namespace cameraReader {

/**
 * @brief QtCameraReader process
 *
 */
template<class View>
class QtCameraReaderProcess : public ImageGilProcessor<View>
{
public:
    typedef typename View::value_type Pixel;
    typedef typename boost::gil::channel_type<View>::type Channel;
    typedef float Scalar;
protected:
    QtCameraReaderPlugin&    _plugin;            ///< Rendering plugin
    QtCameraReaderProcessParams _params;         ///< parameters

public:
    QtCameraReaderProcess( QtCameraReaderPlugin& effect );

    void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );

private:
    // Read camera frame
    View& readFrame( View& dst );
};

}
}
}

#include "QtCameraReaderProcess.tcc"

#endif
