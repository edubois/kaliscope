#ifndef _DCRAW_HPP_
#define	_DCRAW_HPP_

#include <boost/gil/gil_all.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

namespace dcraw
{
    /**
     * @brief read raw data
     * @param user_quality user interpolation quality [0-3]
     */
    boost::shared_array<ushort> getRawData( const int interpolationQuality = 3 );

    /**
     * @brief open a raw image
     * @warning need to be called before all
     */
    bool openRaw( const boost::filesystem::path & filename );

    /**
     * @brief cleanup dcraw internal data
     */
    void cleanup();

    /**
     * @brief read raw image header
     * @param[out] w width
     * @param[out] h height
     */
    void readDimensions( int & w, int & h );

    /**
     * @brief read raw image header
     * @param[in] filename input filename
     * @param[out] w width
     * @param[out] h height
     */
    inline void readDimensions( const boost::filesystem::path & filename, int & w, int & h )
    {
        openRaw( filename );
        readDimensions( w, h );
    }

    /**
     * @brief get number of used channels
     * @return the number of channels in {1,3,4}
     */
    int numberOfChannel();

    /**
     * @brief read raw image
     * @param filename the input filename
     * @param dst the destination view
     * @param interpolationQuality quality of the interpolation in [0-3]
     * @return true or false, true if success
     */
    template<class DView>
    bool readRaw( const boost::filesystem::path & filename, const DView & dst, const int interpolationQuality = 3 )
    {
        int iwidth = 0, iheight = 0;
        openRaw( filename );
        readDimensions( iwidth, iheight );
        boost::shared_array<ushort> ppm2 = getRawData( interpolationQuality );
        bool ret = (ppm2.get() != NULL);
        if ( ret )
        {
            switch( numberOfChannel() )
            {
                case 1:
                {
                    boost::gil::gray16_view_t srcView = boost::gil::interleaved_view( iwidth, iheight, reinterpret_cast<boost::gil::gray16_pixel_t*>( ppm2.get() ), sizeof(ushort) * iwidth * 1 );
                    boost::gil::copy_and_convert_pixels( srcView, dst );
                    break;
                }
                case 3:
                {
                    boost::gil::rgb16_view_t srcView = boost::gil::interleaved_view( iwidth, iheight, reinterpret_cast<boost::gil::rgb16_pixel_t*>( ppm2.get() ), sizeof(ushort) * iwidth * 3 );
                    boost::gil::copy_and_convert_pixels( srcView, dst );
                    break;
                }
                case 4:
                {
                    boost::gil::rgba16_view_t srcView = boost::gil::interleaved_view( iwidth, iheight, reinterpret_cast<boost::gil::rgba16_pixel_t*>( ppm2.get() ), sizeof(ushort) * iwidth * 4 );
                    boost::gil::copy_and_convert_pixels( srcView, dst );
                    break;
                }
                default:
                {
                    std::cerr << "Invalid number of channels!" << std::endl;
                    ret = false;
                    break;
                }
            }
        }
        cleanup();
        return ret;
    }
}

#endif
