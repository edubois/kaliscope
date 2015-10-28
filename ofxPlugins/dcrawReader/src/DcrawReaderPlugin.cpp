/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

#include "DcrawReaderPlugin.hpp"
#include "DcrawReaderProcess.hpp"
#include "DcrawReaderDefinitions.hpp"


#include <boost/gil/gil_all.hpp>

namespace tuttle {
namespace plugin {
namespace dcrawReader {


DcrawReaderPlugin::DcrawReaderPlugin( OfxImageEffectHandle handle )
: ReaderPlugin( handle )
{
    _paramInterpQuality = fetchChoiceParam( kParamInterpolationQuality );
}

DcrawReaderProcessParams DcrawReaderPlugin::getProcessParams( const OfxTime time ) const
{
    DcrawReaderProcessParams params;
    params._filepath = getAbsoluteFilenameAt( time );
        params._interpolationQuality  = _paramInterpQuality->getValue();
    return params;
}

void DcrawReaderPlugin::changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName )
{
    ReaderPlugin::changedParam( args, paramName );
}

bool DcrawReaderPlugin::getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod )
{
    int iwidth = 0, iheight = 0;
    dcraw::readDimensions( getAbsoluteFilenameAt( args.time ), iwidth, iheight );
    rod.x1 = 0;
    rod.x2 = iwidth * this->_clipDst->getPixelAspectRatio();
    rod.y1 = 0;
    rod.y2 = iheight;
    return true;
}

void DcrawReaderPlugin::getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences )
{
    ReaderPlugin::getClipPreferences( clipPreferences );

    clipPreferences.setClipBitDepth( *_clipDst, OFX::eBitDepthFloat );
    clipPreferences.setClipComponents( *_clipDst, OFX::ePixelComponentRGB );
    clipPreferences.setPixelAspectRatio( *this->_clipDst, 1.0 );
}

void DcrawReaderPlugin::beginSequenceRender( const OFX::BeginSequenceRenderArguments& args )
{
    ReaderPlugin::beginSequenceRender( args );
}

/**
 * @brief The overridden render function
 * @param[in]   args     Rendering parameters
 */
void DcrawReaderPlugin::render( const OFX::RenderArguments& args )
{
    ReaderPlugin::render( args );

    // instantiate the render code based on the pixel depth of the dst clip
    OFX::EBitDepth bitDepth         = _clipDst->getPixelDepth();
    OFX::EPixelComponent components = _clipDst->getPixelComponents();

    switch( components )
    {
        case OFX::ePixelComponentRGB:
        {
            doGilRender<DcrawReaderProcess, false, terry::rgb_layout_t>( *this, args, bitDepth );
            return;
        }
        case OFX::ePixelComponentRGBA:
        case OFX::ePixelComponentAlpha:
        case OFX::ePixelComponentCustom:
        case OFX::ePixelComponentNone:
        {
            BOOST_THROW_EXCEPTION( exception::Unsupported()
                    << exception::user() + "Pixel components (" + mapPixelComponentEnumToString(components) + ") not supported by the plugin." );
        }
    }
    BOOST_THROW_EXCEPTION( exception::Unknown() );
}


}
}
}
