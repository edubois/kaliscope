#include "QtCameraReaderPluginFactory.hpp"
#include "QtCameraReaderPlugin.hpp"
#include "CameraReaderDefinitions.hpp"
#include "QtCameraReaderDefinitions.hpp"
#include "ofxsImageEffect.h"

#include <tuttle/plugin/context/ReaderPluginFactory.hpp>

#include <limits>

namespace tuttle {
namespace plugin {
namespace cameraReader {

static const bool kSupportTiles = false;


/**
 * @brief Function called to describe the plugin main features.
 * @param[in, out] desc Effect descriptor
 */
void QtCameraReaderPluginFactory::describe( OFX::ImageEffectDescriptor& desc )
{
	desc.setLabels(
		"QtCameraReader",
		"QtCameraReader",
		"Qt Webcam reader" );
	desc.setPluginGrouping( "djarlabs" );

	// add the supported contexts, only filter at the moment
	desc.addSupportedContext( OFX::eContextReader );
	desc.addSupportedContext( OFX::eContextGeneral );

	// add supported pixel depths
	desc.addSupportedBitDepth( OFX::eBitDepthUByte );
	desc.addSupportedBitDepth( OFX::eBitDepthUShort );
	desc.addSupportedBitDepth( OFX::eBitDepthFloat );

	// plugin flags
	desc.setRenderThreadSafety( OFX::eRenderInstanceSafe );
	desc.setHostFrameThreading( false );
	desc.setSupportsMultiResolution( false );
	desc.setSupportsMultipleClipDepths( true );
	desc.setSupportsMultipleClipPARs( true );
	desc.setSupportsTiles( kSupportTiles );
}

/**
 * @brief Function called to describe the plugin controls and features.
 * @param[in, out]   desc       Effect descriptor
 * @param[in]        context    Application context
 */
void QtCameraReaderPluginFactory::describeInContext( OFX::ImageEffectDescriptor& desc,
                                                  OFX::EContext context )
{
    // Create the mandated output clip
    OFX::ClipDescriptor* dstClip = desc.defineClip( kOfxImageEffectOutputClipName );
    dstClip->addSupportedComponent( OFX::ePixelComponentRGBA );
    dstClip->addSupportedComponent( OFX::ePixelComponentRGB );
    dstClip->addSupportedComponent( OFX::ePixelComponentAlpha );
    dstClip->setSupportsTiles( kSupportTiles );

    OFX::ChoiceParamDescriptor* component = desc.defineChoiceParam( kTuttlePluginChannel );
    component->appendOption( kTuttlePluginChannelAuto );
    component->appendOption( kTuttlePluginChannelGray );
    component->appendOption( kTuttlePluginChannelRGB );
    component->appendOption( kTuttlePluginChannelRGBA );

    component->setLabel( kTuttlePluginChannelLabel );
    component->setDefault( eParamReaderChannelAuto );
    desc.addClipPreferencesSlaveParam( *component );

    OFX::ChoiceParamDescriptor* explicitConversion = desc.defineChoiceParam( kTuttlePluginBitDepth );
    explicitConversion->setLabel( kTuttlePluginBitDepthLabel );
    explicitConversion->appendOption( kTuttlePluginBitDepthAuto );
    explicitConversion->appendOption( kTuttlePluginBitDepth8 );
    explicitConversion->appendOption( kTuttlePluginBitDepth16 );
    explicitConversion->appendOption( kTuttlePluginBitDepth32f );
    explicitConversion->setCacheInvalidation( OFX::eCacheInvalidateValueAll );
    explicitConversion->setAnimates( false );
    desc.addClipPreferencesSlaveParam( *explicitConversion );

    if( OFX::getImageEffectHostDescription()->supportsMultipleClipDepths )
    {
        explicitConversion->setDefault( 0 );
    }
    else
    {
        explicitConversion->setIsSecret( true );
        explicitConversion->setDefault( static_cast<int>( OFX::getImageEffectHostDescription()->getDefaultPixelDepth() ) );
    }

    OFX::ChoiceParamDescriptor* paramCamera = desc.defineChoiceParam( kParamCameraChoice );
    paramCamera->setLabel( kParamCameraChoice );
    paramCamera->setHint( "Available cameras. If nothing in the list, please check your camera connection" );
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    Q_FOREACH( const QCameraInfo & info, availableCameras )
    {
        paramCamera->appendOption( ( info.description() + " (" + info.deviceName() + ")" ).toStdString() );
    }

    OFX::ChoiceParamDescriptor* paramResolution = desc.defineChoiceParam( kParamResolutionChoice );
    paramResolution->setLabel( kParamResolutionChoice );
    paramResolution->setHint( "Available resolutions. If nothing in the list, please check your camera connection" );
    Q_FOREACH( const QCameraInfo & info, availableCameras )
    {
        QCamera camera( info );
        QCameraImageCapture imageCapture( &camera );
        const QList<QSize> res = imageCapture.supportedResolutions();
        Q_FOREACH( const QSize & sz, res )
        {
            paramResolution->appendOption( ( info.deviceName() + ": " + sz.width() + " * " + sz.height() ).toStdString() );
        }
    }

    OFX::ChoiceParamDescriptor* paramFocusMode = desc.defineChoiceParam( kParamFocusMode );
    paramFocusMode->setLabel( kParamFocusMode );
    paramFocusMode->appendOption( "Auto" );
    paramFocusMode->appendOption( "Manual" );
    paramFocusMode->appendOption( "Infinity focus" );
    paramFocusMode->appendOption( "Macro" );
    paramFocusMode->setDefault( eParamFocusModeAuto );

    OFX::ChoiceParamDescriptor* paramAperture = desc.defineChoiceParam( kParamApertureChoice );
    paramAperture->setLabel( kParamApertureChoice );
    paramAperture->setHint( "Available aperture" );
    Q_FOREACH( const QCameraInfo & info, availableCameras )
    {
        QCamera camera( info );
        QCameraExposure *exposure = camera.exposure();
        const QList<qreal> apertures = exposure->supportedApertures();
        Q_FOREACH( const qreal x, apertures )
        {
            paramAperture->appendOption( ( info.deviceName() + ": " + QString("f") + x ).toStdString() );
        }
    }

    OFX::ChoiceParamDescriptor* paramShutterSpeed = desc.defineChoiceParam( kParamShutterSpeedChoice );
    paramShutterSpeed->setLabel( kParamShutterSpeedChoice );
    paramShutterSpeed->setHint( "Available shutter speeds" );
    Q_FOREACH( const QCameraInfo & info, availableCameras )
    {
        QCamera camera( info );
        QCameraExposure *exposure = camera.exposure();
        const QList<qreal> shutterSpeeds = exposure->supportedShutterSpeeds();
        Q_FOREACH( const qreal x, shutterSpeeds )
        {
            paramShutterSpeed->appendOption( ( info.deviceName() + ": " + x ).toStdString() );
        }
    }

    OFX::ChoiceParamDescriptor* paramISOSensitivities = desc.defineChoiceParam( kParamISOChoice );
    paramISOSensitivities->setLabel( kParamISOChoice );
    paramISOSensitivities->setHint( "Available ISOs (sensibility)" );
    Q_FOREACH( const QCameraInfo & info, availableCameras )
    {
        QCamera camera( info );
        QCameraExposure *exposure = camera.exposure();
        const QList<int> isos = exposure->supportedIsoSensitivities();
        Q_FOREACH( const int x, isos )
        {
            paramISOSensitivities->appendOption( ( info.deviceName() + " : " + x ).toStdString() );
        }
    }

    OFX::DoubleParamDescriptor *paramExposureCompensation = desc.defineDoubleParam( kParamExposureCompensation );
    paramExposureCompensation->setLabels( kParamExposureCompensation, kParamExposureCompensation, kParamExposureCompensation );
    paramExposureCompensation->setDefault( 0 );
    paramExposureCompensation->setRange( -20, 20 );
    paramExposureCompensation->setDisplayRange( -20, 20 );

    OFX::BooleanParamDescriptor *paramAutoExposure = desc.defineBooleanParam( kParamAutoExposure );
    paramAutoExposure->setLabels( kParamAutoExposure, kParamAutoExposure, kParamAutoExposure );
    paramAutoExposure->setDefault( true );

    OFX::BooleanParamDescriptor *paramAutoISO = desc.defineBooleanParam( kParamAutoSensibility );
    paramAutoISO->setLabels( kParamAutoSensibility, kParamAutoSensibility, kParamAutoSensibility );
    paramAutoISO->setDefault( true );

    OFX::BooleanParamDescriptor *paramAutoShutterSpeed = desc.defineBooleanParam( kParamAutoShutterSpeed );
    paramAutoShutterSpeed->setLabels( kParamAutoShutterSpeed, kParamAutoShutterSpeed, kParamAutoShutterSpeed );
    paramAutoShutterSpeed->setDefault( true );

    OFX::BooleanParamDescriptor *paramAutoAperture = desc.defineBooleanParam( kParamAutoAperture );
    paramAutoAperture->setLabels( kParamAutoAperture, kParamAutoAperture, kParamAutoAperture );
    paramAutoAperture->setDefault( true );

    OFX::DoubleParamDescriptor *paramOpticalZoomFactor = desc.defineDoubleParam( kParamOpticalZoomFactor );
    paramOpticalZoomFactor->setLabels( kParamOpticalZoomFactor, kParamOpticalZoomFactor, kParamOpticalZoomFactor );
    paramOpticalZoomFactor->setDefault( 0 );
    paramOpticalZoomFactor->setRange( 0, 100 );
    paramOpticalZoomFactor->setDisplayRange( 0, 100 );

    OFX::DoubleParamDescriptor *paramWhiteBalance = desc.defineDoubleParam( kParamWhiteBalance );
    paramWhiteBalance->setLabels( kParamWhiteBalance, kParamWhiteBalance, kParamWhiteBalance );
    paramWhiteBalance->setDefault( 0 );    // Automatic
    paramWhiteBalance->setRange( 0, 12000 );
    paramWhiteBalance->setDisplayRange( 0, 12000 );
}

/**
 * @brief Function called to create a plugin effect instance
 * @param[in] handle  Effect handle
 * @param[in] context Application context
 * @return  plugin instance
 */
OFX::ImageEffect* QtCameraReaderPluginFactory::createInstance( OfxImageEffectHandle handle,
                                                          OFX::EContext context )
{
    return new QtCameraReaderPlugin( handle );
}

}
}
}

