#ifndef _TUTTLE_PLUGIN_QTCAMERAREADER_DEFINITIONS_HPP_
#define _TUTTLE_PLUGIN_QTCAMERAREADER_DEFINITIONS_HPP_

#include <tuttle/plugin/global.hpp>

#include <QtMultimedia/QCameraFocus>

namespace tuttle {
namespace plugin {
namespace cameraReader {

enum EParamFocusMode {
    eParamFocusModeAuto = 0,
    eParamFocusModeManual = 1,
    eParamFocusModeMacro = 2,
    eParamFocusModeInfinity = 3
};

}
}
}

#endif
