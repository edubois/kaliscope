#ifndef _CAMERAVIDEO_HPP_
#define	_CAMERAVIDEO_HPP_

#include <boost/filesystem/path.hpp>

#include <memory>
#include <vector>
#include <cstdint>

namespace tuttle {
namespace io {

/**
 * @brief used to read Margic Lantern MLV raw video format
 */
class CameraVideo
{
public:
    CameraVideo();
    virtual ~CameraVideo();

    void readFrame();

    int frameCount() const
    { return _header->videoFrameCount; }
    
    int width() const
    { return _rawHeader->raw_info.width; }

    int height() const
    { return _rawHeader->raw_info.height; }

private:
    std::unique_ptr<std::vector<uint8_t>> _buffer;  ///< Internal frame buffer
};

}
}

#endif
