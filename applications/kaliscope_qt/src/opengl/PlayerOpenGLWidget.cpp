#include "PlayerOpenGLWidget.hpp"

#include <tuttle/host/attribute/Image.hpp>

#include <QtGui/QSurfaceFormat>

#include <boost/gil/extension/io/jpeg_io.hpp>

namespace kaliscope
{
namespace gui
{
namespace qt
{


PlayerOpenGLWidget::PlayerOpenGLWidget( QWidget * parent )
: QOpenGLWidget( parent )
{
    setGeometry( 0, 0, 1280, 720 );
}

PlayerOpenGLWidget::~PlayerOpenGLWidget()
{
    glDeleteTextures( 1, &_currentTexture );
}

void PlayerOpenGLWidget::initializeGL()
{
    glEnable( GL_TEXTURE_2D ); // Enable texturing

    glGenTextures( 1, &_currentTexture ); // Obtain an id for the texture

    glDisable( GL_TEXTURE_2D ); // Enable texturing
}

void PlayerOpenGLWidget::resizeGL( const int w, const int h )
{
    glViewport( 0, 0, w, h );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0, w, h, 0, -1, 1 ); // Match qt coord origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void PlayerOpenGLWidget::paintGL()
{
    std::unique_lock<std::mutex> lock( _mutexDisplay );

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glColor3f(1,1,1);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, _currentTexture );


    // Draw a textured quad
    float ratio = _frameWidth ? ( _frameHeight / float(_frameWidth) ) : ( height() / float( width() ) );
    glBegin( GL_QUADS );
    if ( width() * ratio <= height() )
    {
        const double w = width();
        const double h = width() * ratio;
        double posX = std::max( 0.0, width() - w ) / 2.0;
        double posY = std::max( 0.0, height() - h ) / 2.0;
        glTexCoord2f( 0, 0 ); glVertex3f( posX, h + posY, 0 );
        glTexCoord2f( 0, 1 ); glVertex3f( posX, posY, 0 );
        glTexCoord2f( 1, 1 ); glVertex3f( posX + w, posY, 0 );
        glTexCoord2f( 1, 0 ); glVertex3f( posX + w, h + posY, 0 );
    }
    else
    {
        ratio = 1.0f / ratio;
        const double w = height() * ratio;
        const double h = height();
        double posX = std::max( 0.0, width() - w ) / 2.0;
        double posY = std::max( 0.0, height() - h ) / 2.0;
        glTexCoord2f( 0, 0 ); glVertex3f( posX, h + posY, 0 );
        glTexCoord2f( 0, 1 ); glVertex3f( posX, posY, 0 );
        glTexCoord2f( 1, 1 ); glVertex3f( posX + w, posY, 0 );
        glTexCoord2f( 1, 0 ); glVertex3f( posX + w, h + posY, 0 );
    }
    glEnd();
    
    glDisable( GL_TEXTURE_2D );
    // We are done, process next
    signalFrameDone( _currentFrameNumber );
}

/**
 * @brief set current texture to frame
 * @param frame a video frame
 */
void PlayerOpenGLWidget::setFrame( const std::size_t frameNumber, const DefaultImageT & frame )
{
    std::unique_lock<std::mutex> lock( _mutexDisplay );

    typedef boost::gil::rgb8_image_t SourceImageT;
    glEnable( GL_TEXTURE_2D ); // Enable texturing

    glBindTexture( GL_TEXTURE_2D, _currentTexture ); // Set as the current texture

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

    const SourceImageT::view_t originalFrameView = frame->getGilView<SourceImageT::view_t>();

    _frameWidth = originalFrameView.width();
    _frameHeight = originalFrameView.height();

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,
                  originalFrameView.width(),
                  originalFrameView.height(), 0,
                  GL_RGB, GL_UNSIGNED_BYTE,
                  boost::gil::interleaved_view_get_raw_data( originalFrameView ) );

    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glDisable( GL_TEXTURE_2D );
    
    _currentFrameNumber = frameNumber;
    update();
}

}
}
}
