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

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

PlayerOpenGLWidget::PlayerOpenGLWidget( QWidget * parent )
: QOpenGLWidget( parent )
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
}

void PlayerOpenGLWidget::resizeGL( const int w, const int h )
{
    glViewport( 0, 0, w, h );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0, w, h, 0, -1, 1 ); // Match qt coord origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    rebuildVertexBuffer( w, h );
}

void PlayerOpenGLWidget::paintGL()
{
    std::unique_lock<std::mutex> lock( _mutexDisplay );
    glColor3f( 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );

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

    // We are done, process next
    signalFrameDone( _currentFrameNumber );
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
    posX = std::max( 0.0f, iw - w ) / 2.0;
    posY = std::max( 0.0f, ih - h ) / 2.0;

    _vertices.clear();
    _vertices.push_back( QVector3D( posX, posY, 0 ) );
    _vertices.push_back( QVector3D( posX, h + posY, 0 ) );
    _vertices.push_back( QVector3D( w + posX, posY, 0 ) );
    _vertices.push_back( QVector3D( w + posX, h + posY, 0 ) );
    _vertices.push_back( QVector3D( w + posX, posY, 0 ) );
    _vertices.push_back( QVector3D( posX, h + posY, 0 ) );

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
    typedef boost::gil::rgb8_image_t SourceImageT;
    const SourceImageT::view_t originalFrameView = frame->getGilView<SourceImageT::view_t>();

    bool videoDimensionsChanged = false;
    if ( originalFrameView.width() != _frameWidth )
    {
        _frameWidth = originalFrameView.width();
        videoDimensionsChanged = true;
    }
    if ( originalFrameView.height() != _frameHeight )
    {
        _frameHeight = originalFrameView.height();
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

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,
                  _frameWidth,
                  _frameHeight, 0,
                  GL_RGB, GL_UNSIGNED_BYTE,
                  boost::gil::interleaved_view_get_raw_data( originalFrameView ) );

    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glDisable( GL_TEXTURE_2D );

    _currentFrameNumber = frameNumber;
    doneCurrent();
    update();
}

}
}
}
