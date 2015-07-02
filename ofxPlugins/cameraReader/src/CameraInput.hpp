#ifndef _CAMERAINPUT_HPP_
#define	_CAMERAINPUT_HPP_

#include <boost/filesystem/path.hpp>

#include <memory>
#include <vector>
#include <cstdint>

namespace tuttle {
namespace io {

/**
 * @brief used to read frames from webcam
 */
class CameraInput
{
public:
    CameraInput();
    virtual ~CameraInput();

    void readFrame();

    int frameCount() const
    { return 0; }
    
    int width() const
    { return 0; }

    int height() const
    { return 0; }

private:
    std::unique_ptr<std::vector<uint8_t>> _buffer;  ///< Internal frame buffer
};

}
}

#endif
