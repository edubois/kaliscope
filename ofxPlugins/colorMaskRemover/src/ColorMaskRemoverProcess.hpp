#ifndef _TUTTLE_PLUGIN_FILTERCLEANER_PROCESS_HPP_
#define _TUTTLE_PLUGIN_FILTERCLEANER_PROCESS_HPP_

#include <tuttle/plugin/ImageGilFilterProcessor.hpp>

namespace tuttle {
namespace plugin {
namespace colorMaskRemover {

/**
 * @brief ColorMaskRemover process
 *
 */
template<class View>
class ColorMaskRemoverProcess : public ImageGilFilterProcessor<View>
{
public:
	typedef typename View::value_type Pixel;
	typedef typename boost::gil::channel_type<View>::type Channel;
	typedef float Scalar;
protected:
    ColorMaskRemoverPlugin&    _plugin;            ///< Rendering plugin
	ColorMaskRemoverProcessParams<Scalar> _params; ///< parameters

public:
    ColorMaskRemoverProcess( ColorMaskRemoverPlugin& effect );

	void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );
};

}
}
}

#include "ColorMaskRemoverProcess.tcc"

#endif
