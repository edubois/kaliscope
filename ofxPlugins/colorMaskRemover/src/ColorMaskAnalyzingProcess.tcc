#include "ColorMaskRemoverAlgorithm.hpp"
#include "ColorMaskRemoverPlugin.hpp"

#include <boost/gil/gil_all.hpp>
#include <terry/colorspace/layout/all.hpp>

namespace tuttle {
namespace plugin {
namespace colorMaskRemover {

template<class View>
ColorMaskAnalyzingProcess<View>::ColorMaskAnalyzingProcess( ColorMaskRemoverPlugin &effect )
: ImageGilFilterProcessor<View>( effect, eImageOrientationIndependant )
, _plugin( effect )
, _redFilterColor( 0.0 )
, _greenFilterColor( 0.0 )
, _blueFilterColor( 0.0 )
{
}

template<class View>
void ColorMaskAnalyzingProcess<View>::setup( const OFX::RenderArguments& args )
{
    ImageGilFilterProcessor<View>::setup( args );
    _params = _plugin.getProcessParams( args.renderScale );
    _redFilterColor = 0.0;
    _greenFilterColor = 0.0;
    _blueFilterColor = 0.0;
}

/**
 * @brief Function called by rendering thread each time a process must be done.
 * @param[in] procWindowRoW  Processing window
 */
template<class View>
void ColorMaskAnalyzingProcess<View>::multiThreadProcessImages( const OfxRectI& procWindowRoW )
{
    using namespace boost::gil;
    OfxRectI procWindowOutput = this->translateRoWToOutputClipCoordinates( procWindowRoW );
    const OfxPointI procWindowSize = {
            procWindowRoW.x2 - procWindowRoW.x1,
            procWindowRoW.y2 - procWindowRoW.y1 };

    rgb32f_pixel_t wpix;
    const double vmax = channel_traits<bits32f>::max_value();
    for( int y = procWindowOutput.y1; y < procWindowOutput.y2; ++y )
    {
        typename View::x_iterator src_it = this->_srcView.x_at( procWindowOutput.x1, y );
        typename View::x_iterator dst_it = this->_dstView.x_at( procWindowOutput.x1, y );
        for( int x = procWindowOutput.x1; x < procWindowOutput.x2; ++x, ++src_it, ++dst_it )
        {
            color_convert( *src_it, wpix );
            const double r = get_color( wpix, red_t() );
            const double g = get_color( wpix, green_t() );
            const double b = get_color( wpix, blue_t() );
            if ( r > _redFilterColor )
            {
                _redFilterColor = r / vmax;
            }
            if ( g > _greenFilterColor )
            {
                _greenFilterColor = g / vmax;
            }
            if ( b > _blueFilterColor )
            {
                _blueFilterColor = b / vmax;
            }
            color_convert( wpix, *dst_it );
        }
        if( this->progressForward( procWindowSize.x ) )
            return;
    }
}

template<class View>
void ColorMaskAnalyzingProcess<View>::postProcess()
{
    this->progressEnd();
    _plugin.notifyRGBFilterColor( _redFilterColor, _greenFilterColor, _blueFilterColor );
}

}
}
}
