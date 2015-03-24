#include "XimeaCameraReaderPlugin.hpp"
#include "XimeaCameraReaderProcess.hpp"
#include "XimeaCameraReaderDefinitions.hpp"

#include <boost/gil/gil_all.hpp>

namespace tuttle {
namespace plugin {
namespace ximeaCameraReader {


XimeaCameraReaderPlugin::XimeaCameraReaderPlugin( OfxImageEffectHandle handle )
: ImageEffectGilPlugin( handle )
{
//    _clipSrcMatte = fetchClip( kClipMatte );
}

XimeaCameraReaderProcessParams<XimeaCameraReaderPlugin::Scalar> XimeaCameraReaderPlugin::getProcessParams( const OfxPointD& renderScale ) const
{
	XimeaCameraReaderProcessParams<Scalar> params;
	return params;
}

void XimeaCameraReaderPlugin::changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName )
{
//    if( paramName == kParamHelpButton )
//    {
//        sendMessage( OFX::Message::eMessageMessage,
//                     "", // No XML resources
//                     kParamHelpString );
//    }
}

//bool XimeaCameraReaderPlugin::getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod )
//{
//	XimeaCameraReaderProcessParams<Scalar> params = getProcessParams();
//	OfxRectD srcRod = _clipSrc->getCanonicalRod( args.time );
//
//	switch( params._border )
//	{
//		case eParamBorderPadded:
//			rod.x1 = srcRod.x1 + 1;
//			rod.y1 = srcRod.y1 + 1;
//			rod.x2 = srcRod.x2 - 1;
//			rod.y2 = srcRod.y2 - 1;
//			return true;
//		default:
//			break;
//	}
//	return false;
//}
//
//void XimeaCameraReaderPlugin::getRegionsOfInterest( const OFX::RegionsOfInterestArguments& args, OFX::RegionOfInterestSetter& rois )
//{
//	XimeaCameraReaderProcessParams<Scalar> params = getProcessParams();
//	OfxRectD srcRod = _clipSrc->getCanonicalRod( args.time );
//
//	OfxRectD srcRoi;
//	srcRoi.x1 = srcRod.x1 - 1;
//	srcRoi.y1 = srcRod.y1 - 1;
//	srcRoi.x2 = srcRod.x2 + 1;
//	srcRoi.y2 = srcRod.y2 + 1;
//	rois.setRegionOfInterest( *_clipSrc, srcRoi );
//}

bool XimeaCameraReaderPlugin::isIdentity( const OFX::RenderArguments& args, OFX::Clip*& identityClip, double& identityTime )
{
//	XimeaCameraReaderProcessParams<Scalar> params = getProcessParams();
//	if( params._in == params._out )
//	{
//		identityClip = _clipSrc;
//		identityTime = args.time;
//		return true;
//	}
	return false;
}

/**
 * @brief The overridden render function
 * @param[in]   args     Rendering parameters
 */
void XimeaCameraReaderPlugin::render( const OFX::RenderArguments &args )
{
	doGilRender<XimeaCameraReaderProcess>( *this, args );
}


}
}
}
