#include "CameraVideo.hpp"

#include <tuttle/plugin/exceptions.hpp>

#include <fstream>
#include <iostream>

namespace tuttle {
namespace io {

CameraVideo::CameraVideo()
{
}

CameraVideo::~CameraVideo()
{
}

void CameraVideo::readFrame()
{
    // Check buffer allocation
    if ( !_buffer )
    {
        BOOST_THROW_EXCEPTION( exception::Failed()
                                << exception::user() + "Not enough memory to allocate a frame!" );
    }
}

}
}
