#ifndef _TUTTLE_PLUGIN_XIMEACAMERAREADER_PROCESS_HPP_
#define _TUTTLE_PLUGIN_XIMEACAMERAREADER_PROCESS_HPP_

#include <tuttle/plugin/ImageGilFilterProcessor.hpp>

namespace tuttle {
namespace plugin {
namespace ximeaCameraReader {

/**
 * @brief XimeaCameraReader process
 *
 */
template<class View>
class XimeaCameraReaderProcess : public ImageGilFilterProcessor<View>
{
public:
	typedef typename View::value_type Pixel;
	typedef typename boost::gil::channel_type<View>::type Channel;
	typedef float Scalar;
protected:
    XimeaCameraReaderPlugin&    _plugin;            ///< Rendering plugin
	XimeaCameraReaderProcessParams<Scalar> _params; ///< parameters

public:
    XimeaCameraReaderProcess( XimeaCameraReaderPlugin& effect );

	void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );
};

}
}
}

#include "XimeaCameraReaderProcess.tcc"

#endif
