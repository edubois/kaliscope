#include "QtCameraReaderPlugin.hpp"
#include "QtCameraReaderProcess.hpp"

namespace tuttle {
namespace plugin {
namespace cameraReader {

QtCameraReaderPlugin::QtCameraReaderPlugin( OfxImageEffectHandle handle )
: CameraReaderPlugin( handle )
{
    fillParameters();
}

QtCameraReaderPlugin::~QtCameraReaderPlugin()
{}

void QtCameraReaderPlugin::createNewCamera( const QCameraInfo & cameraInfo )
{
    _camera.reset( new QCamera( cameraInfo ) );
    _camera->setCaptureMode( QCamera::CaptureStillImage );
    _camInfo.reset( new QCameraInfo( _camera.get() ) );
    _imageCapture.reset( new QCameraImageCapture( _camera.get() ) );
    _imageCapture->setCaptureDestination( QCameraImageCapture::CaptureToBuffer );
    _imageCapture->setBufferFormat( QVideoFrame::Format_RGB24 );
    _exposureControl = _camera->exposure();
    _focusControl = _camera->focus();
    _imageControl = _camera->imageProcessing();
    _exposureControl->setFlashMode( QCameraExposure::FlashOff );
}

void QtCameraReaderPlugin::fillParameters()
{
    OFX::InstanceChangedArgs dummyArg;
    changedParam( dummyArg, kParamCameraChoice );
    changedParam( dummyArg, kParamFocusMode );
    changedParam( dummyArg, kParamApertureChoice );
    changedParam( dummyArg, kParamShutterSpeedChoice );
    changedParam( dummyArg, kParamISOChoice );
    changedParam( dummyArg, kParamExposureCompensation );
    changedParam( dummyArg, kParamAutoExposure );
    changedParam( dummyArg, kParamAutoSensibility );
    changedParam( dummyArg, kParamAutoShutterSpeed );
    changedParam( dummyArg, kParamAutoAperture );
    changedParam( dummyArg, kParamOpticalZoomFactor );
    changedParam( dummyArg, kParamWhiteBalance );
}

bool QtCameraReaderPlugin::getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod )
{
    QSize imgSize( 0, 0 );
    if ( _paramResolution->getValue() >= 0 && _paramResolution->getValue() < _imageCapture->supportedResolutions().size() )
    {
        imgSize = _imageCapture->supportedResolutions().at( _paramResolution->getValue() );
    }
    rod.x1 = 0;
    rod.x2 = imgSize.width() * this->_clipDst->getPixelAspectRatio();
    rod.y1 = 0;
    rod.y2 = imgSize.height();
    return true;
}

QtCameraReaderProcessParams QtCameraReaderPlugin::getProcessParams( const OfxTime time ) const
{
    QtCameraReaderProcessParams params;
    params.camera = _camera;
    params.capture = _imageCapture;
    return params;
}

void QtCameraReaderPlugin::changedParam( const OFX::InstanceChangedArgs& args, const std::string& paramName )
{
    if ( paramName == kParamCameraChoice )
    {
        const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
        if ( _paramCamera->getValue() >= 0 && _paramCamera->getValue() < availableCameras.size() )
        {
            createNewCamera( availableCameras.at( _paramCamera->getValue() ) );
        }
        else
        {
            createNewCamera( QCameraInfo::defaultCamera() );
        }
    }
    else if ( paramName == kParamFocusMode )
    {
        switch( static_cast<EParamFocusMode>( _paramFocusMode->getValue() ) )
        {
            case eParamFocusModeAuto:
            {
                _focusControl->setFocusMode( QCameraFocus::AutoFocus );
                break;
            }
            case eParamFocusModeManual:
            {
                _focusControl->setFocusMode( QCameraFocus::ManualFocus );
                break;
            }
            case eParamFocusModeMacro:
            {
                _focusControl->setFocusMode( QCameraFocus::MacroFocus );
                break;
            }
            case eParamFocusModeInfinity:
            {
                _focusControl->setFocusMode( QCameraFocus::InfinityFocus );
                break;
            }
        }
    }
    else if ( paramName == kParamApertureChoice )
    {
        const QList<qreal> apertures = _exposureControl->supportedApertures();
        if ( _paramAperture->getValue() >= 0 && _paramAperture->getValue() < apertures.size() )
        {
            const qreal aperture = apertures.at( _paramAperture->getValue() );
            _exposureControl->setManualAperture( aperture );
        }
    }
    else if ( paramName == kParamShutterSpeedChoice )
    {
        const QList<qreal> shutterSpeeds = _exposureControl->supportedShutterSpeeds();
        if ( _paramShutterSpeed->getValue() >= 0 && _paramShutterSpeed->getValue() < shutterSpeeds.size() )
        {
            const qreal shutterSpeed = shutterSpeeds.at( _paramShutterSpeed->getValue() );
            _exposureControl->setManualShutterSpeed( shutterSpeed );
        }
    }
    else if ( paramName == kParamISOChoice )
    {
        const QList<int> isoSensitivities = _exposureControl->supportedIsoSensitivities();
        if ( _paramISOSensitivity->getValue() >= 0 && _paramISOSensitivity->getValue() < isoSensitivities.size() )
        {
            const int iso = isoSensitivities.at( _paramISOSensitivity->getValue() );
            _exposureControl->setManualIsoSensitivity( iso );
        }
    }
    else if ( paramName == kParamExposureCompensation )
    {
        _exposureControl->setExposureCompensation( _paramExposureCompensation->getValue() );
    }
    else if ( paramName == kParamAutoExposure )
    {
        if ( _paramAutoExposure->getValue() )
        {
            _exposureControl->setExposureMode( QCameraExposure::ExposureAuto );
        }
        else
        {
            _exposureControl->setExposureMode( QCameraExposure::ExposureManual );
        }
    }
    else if ( paramName == kParamAutoSensibility )
    {
        if ( _paramAutoISO->getValue() )
        {
            _exposureControl->setAutoIsoSensitivity();
        }
        else
        {
            _exposureControl->setManualIsoSensitivity( _exposureControl->supportedIsoSensitivities().at( _paramAutoISO->getValue() ) );
        }
    }
    else if ( paramName == kParamAutoShutterSpeed )
    {
        if ( _paramAutoShutterSpeed->getValue() )
        {
            _exposureControl->setAutoShutterSpeed();
        }
        else
        {
            _exposureControl->setManualShutterSpeed( _exposureControl->supportedShutterSpeeds().at( _paramShutterSpeed->getValue() ) );
        }
    }
    else if ( paramName == kParamAutoAperture )
    {
        if ( _paramAutoExposure->getValue() )
        {
            _exposureControl->setAutoAperture();
        }
        else
        {
            if ( _paramAperture->getValue() < 0 )
            {
                _exposureControl->setAutoAperture();
            }
            else if ( _paramAperture->getValue() < _exposureControl->supportedApertures().size() )
            {
                _exposureControl->setManualAperture( _exposureControl->supportedApertures().at( _paramAperture->getValue() ) );
            }
        }
    }
    else if ( paramName == kParamOpticalZoomFactor )
    {
        _focusControl->zoomTo( _paramOpticalZoomFactor->getValue(), _focusControl->digitalZoom() );
    }
    else if ( paramName == kParamWhiteBalance )
    {
        if ( _paramWhiteBalance->getValue() == 0.0 )
        {
            _imageControl->setWhiteBalanceMode( QCameraImageProcessing::WhiteBalanceAuto );
        }
        else
        {
            _imageControl->setWhiteBalanceMode( QCameraImageProcessing::WhiteBalanceManual );
            _imageControl->setManualWhiteBalance( _paramWhiteBalance->getValue() );
        }
    }
}

void QtCameraReaderPlugin::render( const OFX::RenderArguments& args )
{
    CameraReaderPlugin::render( args );

    // instantiate the render code based on the pixel depth of the dst clip
    OFX::EBitDepth bitDepth         = _clipDst->getPixelDepth();
    OFX::EPixelComponent components = _clipDst->getPixelComponents();

    switch( components )
    {
        case OFX::ePixelComponentRGBA:
        {
            doGilRender<QtCameraReaderProcess, false, terry::rgba_layout_t>( *this, args, bitDepth );
            return;
        }
        case OFX::ePixelComponentRGB:
        {
            doGilRender<QtCameraReaderProcess, false, terry::rgb_layout_t>( *this, args, bitDepth );
            return;
        }
        case OFX::ePixelComponentAlpha:
        {
            doGilRender<QtCameraReaderProcess, false, terry::gray_layout_t>( *this, args, bitDepth );
            return;
        }
        case OFX::ePixelComponentCustom:
        case OFX::ePixelComponentNone:
        {
            BOOST_THROW_EXCEPTION( exception::Unsupported()
                    << exception::user() + "Pixel components (" + mapPixelComponentEnumToString(components) + ") not supported by the plugin." );
        }
        default:
            BOOST_THROW_EXCEPTION( exception::Unknown() );
    }
}

}
}
}
