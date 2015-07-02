#include "CameraInput.hpp"

#include <tuttle/plugin/exceptions.hpp>

#include <fstream>
#include <iostream>

namespace tuttle {
namespace io {

CameraInput::CameraInput() {
}

CameraInput::~CameraInput() {
}

void CameraInput::readFrame() {
    // Check buffer allocation
    if (!_buffer) {
        BOOST_THROW_EXCEPTION(exception::Failed()
                << exception::user() + "Not enough memory to allocate a frame!");
    }
}

}
}
