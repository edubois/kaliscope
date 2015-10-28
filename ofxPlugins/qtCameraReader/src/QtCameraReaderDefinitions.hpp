/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

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
