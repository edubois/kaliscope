#include "QtCameraReaderPlugin.hpp"

#include <QtCore/QEventLoop>

namespace tuttle {
namespace plugin {
namespace cameraReader {

static const int kCaptureTimeout( 5000 );

template<class View>
QtCameraReaderProcess<View>::QtCameraReaderProcess( QtCameraReaderPlugin &instance )
: ImageGilProcessor<View>( instance, eImageOrientationFromTopToBottom )
, _plugin( instance )
{
    this->setNoMultiThreading();
}

template<class View>
void QtCameraReaderProcess<View>::setup( const OFX::RenderArguments& args )
{
    using namespace boost::gil;
    ImageGilProcessor<View>::setup( args );
    _params = _plugin.getProcessParams( args.time );
}

/**
 * @brief Function called by rendering thread each time a process must be done.
 * @param[in] procWindowRoW  Processing window
 */
template<class View>
void QtCameraReaderProcess<View>::multiThreadProcessImages( const OfxRectI& procWindowRoW )
{
    using namespace boost::gil;
    readFrame( this->_dstView );
}

template<class View>
View& QtCameraReaderProcess<View>::readFrame( View& dst )
{
    using namespace boost;
    using namespace mpl;
    using namespace boost::gil;

    if ( _params.capture )
    {
        QEventLoop waitLoop;
        waitLoop.connect( _params.capture, &QCameraImageCapture::imageAvailable,
                      [&dst,&waitLoop]( const int id, const QVideoFrame & buffer )
                      {
                          rgb32c_view_t src = interleaved_view( buffer.width(), buffer.height(), reinterpret_cast<const rgb32_pixel_t*>( buffer.bits() ), buffer.bytesPerLine() );
                          copy_and_convert_pixels( src, dst );
                          waitLoop.quit();
                      }
                );

        waitLoop.connect( _params.capture, SIGNAL( error( int, QCameraImageCapture::Error, const QString & ) ), &waitLoop, SLOT( quit() ) );

        _params.capture->capture();
        waitLoop.exec();
    }
    return dst;
}

}
}
}
