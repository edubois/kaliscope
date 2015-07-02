#include "CameraReaderAlgorithm.hpp"
#include "CameraReaderPlugin.hpp"
#include "CameraInput.hpp"

namespace tuttle {
namespace plugin {
namespace cameraReader {

template<class View>
CameraReaderProcess<View>::CameraReaderProcess( CameraReaderPlugin &instance )
: ImageGilProcessor<View>( instance, eImageOrientationFromTopToBottom )
, _plugin( instance )
{
    this->setNoMultiThreading();
}

template<class View>
void CameraReaderProcess<View>::setup( const OFX::RenderArguments& args )
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
void CameraReaderProcess<View>::multiThreadProcessImages( const OfxRectI& procWindowRoW )
{
    using namespace boost::gil;
    readFrame( this->_dstView );
}

template<class View>
View& CameraReaderProcess<View>::readFrame( View& dst )
{
    using namespace boost;
    using namespace mpl;
    using namespace boost::gil;

    _cameraInput.readFrame();

    return dst;
}

}
}
}
