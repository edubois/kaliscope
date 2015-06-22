#ifndef _TUTTLE_PLUGIN_FILTERCLEANER_PROCESS_HPP_
#define _TUTTLE_PLUGIN_FILTERCLEANER_PROCESS_HPP_

#include <tuttle/plugin/ImageGilFilterProcessor.hpp>

namespace tuttle {
namespace plugin {
namespace filterCleaner {

/**
 * @brief FilterCleaner process
 *
 */
template<class View>
class FilterCleanerProcess : public ImageGilFilterProcessor<View>
{
public:
	typedef typename View::value_type Pixel;
	typedef typename boost::gil::channel_type<View>::type Channel;
	typedef float Scalar;
protected:
    FilterCleanerPlugin&    _plugin;            ///< Rendering plugin
	FilterCleanerProcessParams<Scalar> _params; ///< parameters

public:
    FilterCleanerProcess( FilterCleanerPlugin& effect );

	void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );
};

}
}
}

#include "FilterCleanerProcess.tcc"

#endif
