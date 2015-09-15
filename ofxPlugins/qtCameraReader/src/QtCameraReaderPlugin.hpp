#ifndef _KALISCOPE_PLUGIN_QTCAMERAREADER_PLUGIN_HPP_
#define _KALISCOPE_PLUGIN_QTCAMERAREADER_PLUGIN_HPP_

#include "CameraReaderPlugin.hpp"
#include "CameraReaderDefinitions.hpp"
#include "QtCameraReaderDefinitions.hpp"

#include <QtMultimedia/QCameraExposure>
#include <QtMultimedia/QCameraFocus>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraImageCapture>
#include <QtMultimedia/QCameraInfo>

#include <boost/shared_ptr.hpp>

namespace tuttle {
namespace plugin {
namespace cameraReader {

struct QtCameraReaderProcessParams
{
    boost::shared_ptr<QCamera> camera;
    boost::shared_ptr<QCameraImageCapture> capture;
};

/**
 * @brief CameraReader plugin
 */
class QtCameraReaderPlugin : public CameraReaderPlugin
{
public:
    QtCameraReaderPlugin( OfxImageEffectHandle handle );
    virtual ~QtCameraReaderPlugin();

public:
    virtual void changedParam( const OFX::InstanceChangedArgs& args, const std::string& paramName );
    virtual bool getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod );
    virtual void render( const OFX::RenderArguments& args );
    QtCameraReaderProcessParams getProcessParams( const OfxTime time ) const;

private:
    void createNewCamera( const QCameraInfo & cameraInfo );
    void fillParameters();

private:
    boost::shared_ptr<QCamera> _camera;                   ///< Camera control
    boost::shared_ptr<QCameraInfo> _camInfo;               ///< Camera info
    boost::shared_ptr<QCameraImageCapture> _imageCapture;  ///< Image capture
    QCameraExposure * _exposureControl; ///< Exposure control
    QCameraFocus * _focusControl;       ///< Focus control
    QCameraImageProcessing * _imageControl;       ///< Image control
};

}
}
}

#endif
