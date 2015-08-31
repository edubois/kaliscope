#include "PlayerOpenGLWidget.hpp"

#include "openglAdds.hpp"

#include <tuttle/host/attribute/Image.hpp>

#include <QtGui/QSurfaceFormat>
#include <QtGui/QWheelEvent>

namespace kaliscope
{
namespace gui
{
namespace qt
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

PlayerOpenGLWidget::PlayerOpenGLWidget( QWidget * parent )
: QOpenGLWidget( parent )
, _zoomFactor( 1.0 )
, _quadVertexBuffer( QGLBuffer::VertexBuffer )
, _quadTextureCoordsBuffer( QGLBuffer::VertexBuffer )
, _quadIndiceBuffer( QGLBuffer::IndexBuffer )
{
    setGeometry( 0, 0, 1280, 720 );

    _textureCoords.clear();
    _textureCoords.push_back( QVector2D( 0, 1.0f ) );
    _textureCoords.push_back( QVector2D( 0, 0.0f ) );
    _textureCoords.push_back( QVector2D( 1.0f, 1.0f ) );
    _textureCoords.push_back( QVector2D( 1.0f, 0.0f ) );
    _textureCoords.push_back( QVector2D( 1.0f, 1.0f ) );
    _textureCoords.push_back( QVector2D( 0, 0.0f ) );

    _indices.clear();
    _indices.push_back( 0 );
    _indices.push_back( 1 );
    _indices.push_back( 2 );
    _indices.push_back( 3 );
    _indices.push_back( 4 );
    _indices.push_back( 5 );
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

    _quadTextureCoordsBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    _quadTextureCoordsBuffer.create();
    _quadTextureCoordsBuffer.bind();
    _quadTextureCoordsBuffer.allocate( _textureCoords.constData(), _textureCoords.size() * sizeof( QVector2D ) );
    _quadTextureCoordsBuffer.release();

    // Indices buffer init
    _quadIndiceBuffer.setUsagePattern( QGLBuffer::StreamDraw );
    _quadIndiceBuffer.create();
    _quadIndiceBuffer.bind();
    _quadIndiceBuffer.allocate( _indices.constData(), _indices.size() * sizeof( GLuint ) );
    _quadIndiceBuffer.release();

    _programShaderNegative.addShaderFromSourceFile( QGLShader::Vertex, ":/shaders/negative_vert.glsl" );
    _programShaderNegative.addShaderFromSourceFile( QGLShader::Fragment, ":/shaders/negative_frag.glsl" );
    if ( !_programShaderNegative.link() )
    {
        std::cerr << _programShaderNegative.log().toStdString() << std::endl;
    }
    _translation[0] = 0.0f; _translation[1] = 0.0f;
}

void PlayerOpenGLWidget::resizeGL( const int w, const int h )
{
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    const float left = -w / 2.0f;
    const float right = w / 2.0f;
    const float top = h / 2.0f;
    const float bottom = -h / 2.0f;
    glOrtho( left, right, bottom, top, -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    rebuildVertexBuffer( w, h );
}

void PlayerOpenGLWidget::paintGL()
{
    std::unique_lock<std::mutex> lock( _mutexDisplay );
    glColor3f( 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    
    glPushMatrix();
    glTranslatef( _translation[0] * width() / 2.0f, -_translation[1] * height() / 2.0f, 0.0f );
    glScalef( _zoomFactor, _zoomFactor, 1.0 );
    glTranslatef( -_translation[0] * width() / 2.0f, _translation[1] * height() / 2.0f, 0.0f );

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_VERTEX_ARRAY );

    _quadVertexBuffer.bind();
    glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET( 0 ) );

    int vertexLocation = 0;
    if ( _invertColors )
    {
        vertexLocation = _programShaderNegative.attributeLocation( "vertex" );
        _programShaderNegative.setAttributeBuffer( vertexLocation, GL_FLOAT, 0, 3 );
        _programShaderNegative.enableAttributeArray( vertexLocation );
    }

    _quadVertexBuffer.release();

    glBindTexture( GL_TEXTURE_2D, _currentTexture );
    glEnable( GL_TEXTURE_2D );

    if ( _invertColors )
    {
        _programShaderNegative.bind();
        _programShaderNegative.setUniformValue( "myTexture", 0 );
    }

    _quadTextureCoordsBuffer.bind();
    glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET( 0 ) );

    int textCoordsLocation = 0;
    if ( _invertColors )
    {
        textCoordsLocation = _programShaderNegative.attributeLocation( "textureCoords" );
        _programShaderNegative.setAttributeBuffer( textCoordsLocation, GL_FLOAT, 0, 2 );
        _programShaderNegative.enableAttributeArray( textCoordsLocation );
    }

    _quadIndiceBuffer.bind();

    glDrawElements( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, BUFFER_OFFSET( 0 ) );
    if ( _invertColors )
    {
        _programShaderNegative.disableAttributeArray( vertexLocation );
        _programShaderNegative.disableAttributeArray( textCoordsLocation );
        _programShaderNegative.release();
    }

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );

    _quadTextureCoordsBuffer.release();
    _quadIndiceBuffer.release();

    glDisable( GL_TEXTURE_2D );
    glPopMatrix();
}

void PlayerOpenGLWidget::rebuildVertexBuffer( const int iw, const int ih )
{
    // Draw a textured quad
    float ratio = _frameWidth ? ( _frameHeight / float( _frameWidth ) ) : ( ih / float( iw ) );
    float w, h;
    float posX, posY;
    if ( iw * ratio <= ih )
    {
        w = iw;
        h = iw * ratio;
    }
    else
    {
        ratio = 1.0f / ratio;
        w = ih * ratio;
        h = ih;
    }
    posX = std::max( 0.0f, iw - w ) / 2.0 - ( iw / 2.0f );
    posY = std::max( 0.0f, ih - h ) / 2.0 - ( ih / 2.0f );

    _vertices.clear();
    _vertices.push_back( QVector3D( posX, h + posY, 0 ) );
    _vertices.push_back( QVector3D( posX, posY, 0 ) );
    _vertices.push_back( QVector3D( w + posX, h + posY, 0 ) );
    _vertices.push_back( QVector3D( w + posX, posY, 0 ) );
    _vertices.push_back( QVector3D( w + posX, h + posY, 0 ) );
    _vertices.push_back( QVector3D( posX, posY, 0 ) );

    // Vertex buffer init
    if ( _quadVertexBuffer.isCreated() )
    {
        _quadVertexBuffer.destroy();
    }
    _quadVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    _quadVertexBuffer.create();
    _quadVertexBuffer.bind();
    _quadVertexBuffer.allocate( _vertices.constData(), _vertices.size() * sizeof( QVector3D ) );
    _quadVertexBuffer.release();
}

/**
 * @brief set current texture to frame
 * @param frame a video frame
 */
void PlayerOpenGLWidget::setFrame( const std::size_t frameNumber, const DefaultImageT & frame )
{
    std::unique_lock<std::mutex> lock( _mutexDisplay );

    makeCurrent();

    bool videoDimensionsChanged = false;
    if ( frame->getBounds().x2 != _frameWidth )
    {
        _frameWidth = frame->getBounds().x2;
        videoDimensionsChanged = true;
    }
    if ( frame->getBounds().y2 != _frameHeight )
    {
        _frameHeight = frame->getBounds().y2;
        videoDimensionsChanged = true;
    }

    if ( videoDimensionsChanged )
    {
        rebuildVertexBuffer( width(), height() );
    }

    glEnable( GL_TEXTURE_2D ); // Enable texturing

    glBindTexture( GL_TEXTURE_2D, _currentTexture ); // Set as the current texture

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

    int bitType = GL_UNSIGNED_BYTE;
    switch( frame->getBitDepth() )
    {
        case 1:
        {
            bitType = GL_UNSIGNED_BYTE;
            break;
        }
        case 2:
        {
            bitType = GL_UNSIGNED_SHORT;
            break;
        }
        case 4:
        {
            bitType = GL_FLOAT;
            break;
        }
        default:
        {
            std::cerr << "Unhandled bit depth: " << frame->getBitDepth() << std::endl;
            break;
        }
    }

    int channelType = GL_RGB;
    switch( frame->getNbComponents() )
    {
        case 1:
        {
            channelType = GL_LUMINANCE;
            break;
        }
        case 3:
        {
            channelType = GL_RGB;
            break;
        }
        case 4:
        {
            channelType = GL_RGBA;
            break;
        }
        default:
        {
            std::cerr << "Unhandled channel type: " << frame->getNbComponents() << std::endl;
            break;
        }
    }
    
    glTexImage2D( GL_TEXTURE_2D, 0, channelType,
                  _frameWidth,
                  _frameHeight, 0,
                  channelType, bitType,
                  frame->getPixelData() );

    // We are done, process next frame (as early as possible)
    _currentFrameNumber = frameNumber;
    signalFrameDone( _currentFrameNumber );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glDisable( GL_TEXTURE_2D );
    doneCurrent();
    update();
}

void PlayerOpenGLWidget::wheelEvent( QWheelEvent * event )
{
    GLfloat modelMatrix[16];
    GLfloat projMatrix[16];
    GLint viewport[4];
    glGetFloatv( GL_MODELVIEW_MATRIX, modelMatrix );
    glGetFloatv( GL_PROJECTION_MATRIX, projMatrix );
    glGetIntegerv( GL_VIEWPORT, viewport );

    if ( event->delta() > 0)
    {
        _zoomFactor *= 1.1;
    }
    else
    {
        _zoomFactor /= 1.1;
    }

    if ( _zoomFactor > 10.0 )
    {
        _zoomFactor = 10.0;
    }
    else if ( _zoomFactor < 0.001 )
    {
        _zoomFactor = 0.001;
    }

    glhUnProjectf( event->x(), event->y(), 0.0f, modelMatrix, projMatrix, viewport, _translation );

    makeCurrent();
    resizeGL( width(), height() );
    doneCurrent();
    update();
}

}
}
}
