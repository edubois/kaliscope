#include "ColorMaskRemoverAlgorithm.hpp"
#include "ColorMaskRemoverPlugin.hpp"

#include <boost/gil/gil_all.hpp>
#include <terry/colorspace/layout/all.hpp>

namespace tuttle {
namespace plugin {
namespace colorMaskRemover {

template<class View>
ColorMaskRemoverProcess<View>::ColorMaskRemoverProcess( ColorMaskRemoverPlugin &effect )
: ImageGilFilterProcessor<View>( effect, eImageOrientationIndependant )
, _plugin( effect )
{
}

template<class View>
void ColorMaskRemoverProcess<View>::setup( const OFX::RenderArguments& args )
{
    ImageGilFilterProcessor<View>::setup( args );
    _params = _plugin.getProcessParams( args.renderScale );
}

/**
 * @brief Function called by rendering thread each time a process must be done.
 * @param[in] procWindowRoW  Processing window
 */
template<class View>
void ColorMaskRemoverProcess<View>::multiThreadProcessImages( const OfxRectI& procWindowRoW )
{
    using namespace boost::gil;
    ColorMaskRemoverProcessParams<ColorMaskRemoverPlugin::Scalar> params = _plugin.getProcessParams();
    OfxRectI procWindowOutput = this->translateRoWToOutputClipCoordinates( procWindowRoW );
    const OfxPointI procWindowSize = {
            procWindowRoW.x2 - procWindowRoW.x1,
            procWindowRoW.y2 - procWindowRoW.y1 };

    const double redFactor = params.fRedFactor;
    const double greenFactor = params.fGreenFactor;
    const double blueFactor = params.fBlueFactor;
    if ( _params._algo == eParamAlgoYUVReduction )
    {
        using namespace terry::color::layout;
        typedef pixel<bits32f, boost::gil::layout< terry::color::layout::yuv_t> > YUVWorkPixT;
        YUVWorkPixT wpix;
        typedef typename channel_type<YUVWorkPixT>::type YUVValueT;
        rgb32f_pixel_t rgbRefFilterPix( params.fRedFilterColor, params.fGreenFilterColor, params.fBlueFilterColor );
        YUVWorkPixT yuvRefFilterPix;
        color_convert( rgbRefFilterPix, yuvRefFilterPix );
        const double yRef = get_color( yuvRefFilterPix, y_t() );
        const double uRef = get_color( yuvRefFilterPix, u_t() );
        const double vRef = get_color( yuvRefFilterPix, v_t() );

        for( int y = procWindowOutput.y1; y < procWindowOutput.y2; ++y )
        {
            typename View::x_iterator src_it = this->_srcView.x_at( procWindowOutput.x1, y );
            typename View::x_iterator dst_it = this->_dstView.x_at( procWindowOutput.x1, y );
            for( int x = procWindowOutput.x1; x < procWindowOutput.x2; ++x, ++src_it, ++dst_it )
            {
                color_convert( *src_it, wpix );
                const double y = get_color( wpix, y_t() );
                const double u = get_color( wpix, u_t() );
                const double v = get_color( wpix, v_t() );
                get_color( wpix, yuv::y_t() ) = std::min( 1.0, std::max( 0.0, y - ( yRef - y ) ) * redFactor );
                get_color( wpix, yuv::u_t() ) = std::min( 0.436, std::max( -0.436, u - uRef ) * greenFactor );
                get_color( wpix, yuv::v_t() ) = std::min( 0.615, std::max( -0.615, v - vRef ) * blueFactor );
                color_convert( wpix, *dst_it );
            }
            if( this->progressForward( procWindowSize.x ) )
                return;
        }
    }
    else if ( _params._algo == eParamAlgoRGBReduction )
    {
        rgb32f_pixel_t rgbRefFilterPix( params.fRedFilterColor, params.fGreenFilterColor, params.fBlueFilterColor );
        rgb32f_pixel_t wpix;
        // Red part in params.fRedFilterColor
        const double subRedFactor = 1.0 + ( 1.0 / params.fRedFilterColor );
        // Green part in params.fGreenFilterColor
        const double subGreenFactor = 1.0 + ( 1.0 / params.fGreenFilterColor );
        // Blue part in params.fBlueFilterColor
        const double subBlueFactor = 1.0 + ( 1.0 / params.fBlueFilterColor );
        const double vmin = channel_traits<bits32f>::min_value();
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
                if ( params.bInvert )
                {
                    get_color( wpix, red_t() )   = vmax - std::min( vmax, std::max( vmin, ( params.fRedFilterColor - r ) * subRedFactor ) * redFactor );
                    get_color( wpix, green_t() ) = vmax - std::min( vmax, std::max( vmin, ( params.fGreenFilterColor - g ) * subGreenFactor ) * greenFactor );
                    get_color( wpix, blue_t() )  = vmax - std::min( vmax, std::max( vmin, ( params.fBlueFilterColor - b ) * subBlueFactor ) * blueFactor );
                    color_convert( wpix, *dst_it );
                }
                else
                {
                    get_color( wpix, red_t() )   = std::min( vmax, std::max( vmin, ( params.fRedFilterColor - r ) * subRedFactor ) * redFactor );
                    get_color( wpix, green_t() ) = std::min( vmax, std::max( vmin, ( params.fGreenFilterColor - g ) * subGreenFactor ) * greenFactor );
                    get_color( wpix, blue_t() )  = std::min( vmax, std::max( vmin, ( params.fBlueFilterColor - b ) * subBlueFactor ) * blueFactor );
                }
                color_convert( wpix, *dst_it );
            }
            if( this->progressForward( procWindowSize.x ) )
                return;
        }
    }
}

}
}
}
