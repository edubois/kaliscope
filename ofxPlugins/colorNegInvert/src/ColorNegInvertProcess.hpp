/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

#ifndef _TUTTLE_PLUGIN_COLORNEGINVERT_PROCESS_HPP_
#define _TUTTLE_PLUGIN_COLORNEGINVERT_PROCESS_HPP_

#include <tuttle/plugin/ImageGilFilterProcessor.hpp>

namespace tuttle {
namespace plugin {
namespace colorNegInvert {

/**
 * @brief ColorNegInvert process
 *
 */
template<class View>
class ColorNegInvertProcess : public ImageGilFilterProcessor<View>
{
public:
	typedef typename View::value_type Pixel;
	typedef typename boost::gil::channel_type<View>::type Channel;
	typedef float Scalar;
protected:
    ColorNegInvertPlugin&    _plugin;            ///< Rendering plugin
    ColorNegInvertProcessParams<Scalar> _params; ///< parameters

public:
    ColorNegInvertProcess( ColorNegInvertPlugin& effect );

	void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );
};

}
}
}

#include "ColorNegInvertProcess.tcc"

#endif
