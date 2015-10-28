/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

#include "DcrawReaderAlgorithm.hpp"
#include "DcrawReaderPlugin.hpp"
#include "dcraw.hpp"

namespace tuttle {
namespace plugin {
namespace dcrawReader {

template<class View>
DcrawReaderProcess<View>::DcrawReaderProcess( DcrawReaderPlugin &instance )
: ImageGilProcessor<View>( instance, eImageOrientationFromTopToBottom )
, _plugin( instance )
{
    this->setNoMultiThreading();
}

template<class View>
void DcrawReaderProcess<View>::setup( const OFX::RenderArguments& args )
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
void DcrawReaderProcess<View>::multiThreadProcessImages( const OfxRectI& procWindowRoW )
{
    using namespace boost::gil;
    readFrame( this->_dstView );
}

template<class View>
View& DcrawReaderProcess<View>::readFrame( View& dst )
{
    dcraw::readRaw( _params._filepath, dst, _params._interpolationQuality );
    return dst;
}

}
}
}
