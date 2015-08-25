#ifndef _TUTTLE_PLUGIN_COLORNEGINVERTANALYZING_PROCESS_HPP_
#define _TUTTLE_PLUGIN_COLORNEGINVERTANALYZING_PROCESS_HPP_

#include <tuttle/plugin/ImageGilFilterProcessor.hpp>
#include <boost/atomic.hpp>

namespace tuttle {
namespace plugin {
namespace colorNegInvert {

/**
 * @brief ColorNegInvert process
 *
 */
template<class View>
class ColorNegInvertAnalyzingProcess : public ImageGilFilterProcessor<View>
{
public:
	typedef typename View::value_type Pixel;
	typedef typename boost::gil::channel_type<View>::type Channel;
	typedef float Scalar;
protected:
    ColorNegInvertPlugin&    _plugin;            ///< Rendering plugin
    ColorNegInvertProcessParams<Scalar> _params; ///< parameters
    boost::atomic<double> _redFilterColor;
    boost::atomic<double> _greenFilterColor;
    boost::atomic<double> _blueFilterColor;

public:
    ColorNegInvertAnalyzingProcess( ColorNegInvertPlugin& effect );

    void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );

    void postProcess();
};

}
}
}

#include "ColorNegInvertAnalyzingProcess.tcc"

#endif
