#ifndef _TUTTLE_PLUGIN_DCRAWREADER_PROCESS_HPP_
#define _TUTTLE_PLUGIN_DCRAWREADER_PROCESS_HPP_

#include <tuttle/plugin/ImageGilProcessor.hpp>

namespace tuttle {
namespace plugin {
namespace dcrawReader {

/**
 * @brief DcrawReader process
 *
 */
template<class View>
class DcrawReaderProcess : public ImageGilProcessor<View>
{
public:
    typedef typename View::value_type Pixel;
    typedef typename boost::gil::channel_type<View>::type Channel;
    typedef float Scalar;
protected:
    DcrawReaderPlugin&    _plugin;            ///< Rendering plugin
    DcrawReaderProcessParams _params;         ///< parameters

public:
    DcrawReaderProcess( DcrawReaderPlugin& effect );

    void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );

private:
    // Read camera frame
    View& readFrame( View& dst );
};

}
}
}

#include "DcrawReaderProcess.tcc"

#endif
