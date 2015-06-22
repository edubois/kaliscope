#ifndef _TERRY_COLOR_LAYOUT_YUV_HPP_
#define	_TERRY_COLOR_LAYOUT_YUV_HPP_

#include <boost/gil/color_convert.hpp>

namespace terry {
namespace color {
namespace layout {

/// \addtogroup ColorNameModel
/// \{
namespace yuv
{
/// \brief Luminance
struct y_t {};
/// \brief U
struct u_t {};
/// \brief V
struct v_t {};
}
/// \}

/// \ingroup ColorSpaceModel
typedef boost::mpl::vector3<
		yuv::y_t,
		yuv::u_t,
		yuv::v_t
	> yuv_t;

}
}
}

namespace boost
{
namespace gil
{

using namespace terry::color::layout;
using namespace terry::color::layout::yuv;

/**
 * @brief Unfortunately RGBA to RGBA must be explicitly provided - otherwise we get ambiguous specialization error.
 */
template <>
struct default_color_converter_impl<terry::color::layout::yuv_t, terry::color::layout::yuv_t> {
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const {
        static_for_each( src, dst, default_channel_converter() );
    }
};

/**
 * @brief RGB -> YUV
 */
template <>
struct default_color_converter_impl<rgb_t, terry::color::layout::yuv_t> {
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const {
        typedef typename channel_type<P1>::type T1;
        const double r = ( get_color( src, red_t() ) - double( channel_traits<T1>::min_value() ) ) / ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) );
        const double g = ( get_color( src, green_t() ) - double( channel_traits<T1>::min_value() ) ) / ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) );
        const double b = ( get_color( src, blue_t() ) - double( channel_traits<T1>::min_value() ) ) / ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) );
        const double y = r * 0.299 + g * 0.587 + b * 0.114;

        get_color( dst, y_t() ) =
            channel_convert<typename color_element_type<P2, y_t >::type>( bits32f( y ) );
        get_color( dst, u_t() ) =
            channel_convert<typename color_element_type<P2, u_t >::type>( bits32f( ( ( b - y ) * 0.492 ) ) );
        get_color( dst, v_t() ) =
            channel_convert<typename color_element_type<P2, v_t >::type>( bits32f( ( ( r - y ) * 0.877 ) ) );
    }
};

/**
 * @brief RGBA -> YUV
 */
template <>
struct default_color_converter_impl<rgba_t, yuv_t> {
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const {
        typedef typename channel_type<P1>::type T1;
        const double alphaFactor = ( get_color( src, alpha_t() ) - double( channel_traits<T1>::min_value() ) ) / ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) );
        const double r = ( get_color( src, red_t() ) - double( channel_traits<T1>::min_value() ) ) / ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) );
        const double g = ( get_color( src, green_t() ) - double( channel_traits<T1>::min_value() ) ) / ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) );
        const double b = ( get_color( src, blue_t() ) - double( channel_traits<T1>::min_value() ) ) / ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) );
        const double y = r * 0.299 + g * 0.587 + b * 0.114;
        
        get_color( dst, y_t() ) =
            channel_convert<typename color_element_type<P2, y_t >::type>( bits32f( alphaFactor * y ) );
        get_color( dst, u_t() ) =
            channel_convert<typename color_element_type<P2, u_t >::type>( bits32f( alphaFactor * ( ( b - y ) * 0.492 ) ) );
        get_color( dst, v_t() ) =
            channel_convert<typename color_element_type<P2, v_t >::type>( bits32f( alphaFactor * ( ( r - y ) * 0.877 ) ) );
    }
};


/**
 * @brief YUV -> RGB
 */
template <>
struct default_color_converter_impl<terry::color::layout::yuv_t, rgb_t> {
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const {
        using namespace terry::color::layout;
        typedef typename channel_type<P1>::type T1;
        // y, u, v must be in [0;1]
        const double y = get_color( src, y_t() );
        const double u = get_color( src, u_t() );
        const double v = get_color( src, v_t() );
        get_color( dst, red_t() ) =
            channel_convert<typename color_element_type<P2, red_t >::type>( bits32f( ( y + ( 1.13983 * v ) ) * ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) ) + double( channel_traits<T1>::min_value() ) ) );
        get_color( dst, green_t() ) =
            channel_convert<typename color_element_type<P2, green_t >::type>( bits32f( ( y - ( 0.39465 * u ) - ( 0.58060 * v ) ) * ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) ) + double( channel_traits<T1>::min_value() ) ) );
        get_color( dst, blue_t() ) =
            channel_convert<typename color_element_type<P2, blue_t >::type>( bits32f( ( y + ( 2.03211 * u ) ) * ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) ) + double( channel_traits<T1>::min_value() ) ) );
    }
};

/**
 * @brief YUV -> RGBA
 */
template <>
struct default_color_converter_impl<terry::color::layout::yuv_t, rgba_t> {
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const {
        typedef typename channel_type<P1>::type T1;
        // y, u, v must be in [0;1]
        const double y = get_color( src, y_t() );
        const double u = get_color( src, u_t() );
        const double v = get_color( src, v_t() );
        get_color( dst, red_t() ) =
            channel_convert<typename color_element_type<P2, red_t >::type>( bits32f( ( y + ( 1.13983 * v ) ) * ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) ) + double( channel_traits<T1>::min_value() ) ) );
        get_color( dst, green_t() ) =
            channel_convert<typename color_element_type<P2, green_t >::type>( bits32f( ( y - ( 0.39465 * u ) - ( 0.58060 * v ) ) * ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) ) + double( channel_traits<T1>::min_value() ) ) );
        get_color( dst, blue_t() ) =
            channel_convert<typename color_element_type<P2, blue_t >::type>( bits32f( ( y + ( 2.03211 * u ) ) * ( double( channel_traits<T1>::max_value() - double( channel_traits<T1>::min_value() ) ) ) + double( channel_traits<T1>::min_value() ) ) );
        get_color( dst, alpha_t() ) =
            channel_convert<typename color_element_type<P2, alpha_t >::type>( bits32f( channel_traits<T1>::max_value() ) );
    }
};


/**
 * @brief GRAY -> YUV
 */
template <>
struct default_color_converter_impl<gray_t, yuv_t> {
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const {
        typedef typename channel_type<P2>::type T2;
        get_color( dst, y_t() ) =
            channel_convert<typename color_element_type<P2, y_t >::type>( get_color( src, gray_color_t() ) );
        get_color( dst, u_t() ) =
            channel_convert<typename color_element_type<P2, u_t >::type>( channel_traits<T2>::min_value() );
        get_color( dst, v_t() ) =
            channel_convert<typename color_element_type<P2, v_t >::type>( channel_traits<T2>::min_value() );
    }
};


/**
 * @brief YUV -> GRAY
 */
template <>
struct default_color_converter_impl<terry::color::layout::yuv_t, gray_t>
{
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const
    {
        using namespace terry::color::layout;
        get_color( dst, gray_color_t() ) =
            channel_convert<typename color_element_type<P2, gray_color_t >::type>( get_color( src, y_t() ) );
    }
};

}
}

#endif
