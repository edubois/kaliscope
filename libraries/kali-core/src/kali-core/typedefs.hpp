#ifndef _KALI_CORE_TYPEDEFS_HPP_
#define	_KALI_CORE_TYPEDEFS_HPP_

#include <boost/gil/typedefs.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/shared_ptr.hpp>

namespace tuttle {
namespace host {
namespace attribute {
class Image;
}
}
}

namespace kaliscope
{

typedef ::boost::shared_ptr<tuttle::host::attribute::Image> DefaultImageT;

}


#endif

