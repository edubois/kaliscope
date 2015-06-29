#ifndef _TUTTLE_PLUGIN_COLORMASKANALYZING_PROCESS_HPP_
#define _TUTTLE_PLUGIN_COLORMASKANALYZING_PROCESS_HPP_

#include <tuttle/plugin/ImageGilFilterProcessor.hpp>
#include <boost/atomic.hpp>

namespace tuttle {
namespace plugin {
namespace colorMaskRemover {

/**
 * @brief ColorMaskRemover process
 *
 */
template<class View>
class ColorMaskAnalyzingProcess : public ImageGilFilterProcessor<View>
{
public:
	typedef typename View::value_type Pixel;
	typedef typename boost::gil::channel_type<View>::type Channel;
	typedef float Scalar;
protected:
    ColorMaskRemoverPlugin&    _plugin;            ///< Rendering plugin
    ColorMaskRemoverProcessParams<Scalar> _params; ///< parameters
    boost::atomic<double> _redFilterColor;
    boost::atomic<double> _greenFilterColor;
    boost::atomic<double> _blueFilterColor;

public:
    ColorMaskAnalyzingProcess( ColorMaskRemoverPlugin& effect );

    void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );

    void postProcess();
};

}
}
}

#include "ColorMaskAnalyzingProcess.tcc"

#endif
