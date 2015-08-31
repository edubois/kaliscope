#ifndef _PLAYEROPENGLWIDGET_HPP_
#define	_PLAYEROPENGLWIDGET_HPP_

#include <kali-core/typedefs.hpp>

#include <QtWidgets/QOpenGLWidget>
#include <QtOpenGL/QGLShaderProgram>
#include <QtOpenGL/QGLBuffer>
#include <QtCore/QVector>

#include <boost/signals2.hpp>
#include <thread>
#include <mutex>

namespace kaliscope
{
namespace gui
{
namespace qt
{

class PlayerOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    PlayerOpenGLWidget( QWidget * parent = nullptr );
    virtual ~PlayerOpenGLWidget();

    /**
     * @brief set current texture to frame
     * @param frameNumber the frame number
     * @param frame a video frame
     */
    void setFrame( const std::size_t frameNumber, const DefaultImageT & frame );

    /**
     * @brief invert display colors (negative) or not
     * @param negative true or false
     */
    void setInvertColors( const bool negative = true )
    { _invertColors = negative; }

public:
    boost::signals2::signal<void( const std::size_t nFrame )> signalFrameDone;

protected:
    /**
     * @brief initialize opengl view
     */
    void initializeGL() override;

    /**
     * @rebuild vertex buffer
     * @param w width
     * @param h height
     */
    void rebuildVertexBuffer( const int w, const int h );

    /**
     * @brief resize opengl view
     */
    void resizeGL( int w, int h ) override;

    /**
     * @brief paint current video frame
     */
    void paintGL() override;

private:
    void wheelEvent(QWheelEvent * event);

private:
    std::mutex _mutexDisplay;       ///< Locker
    float _translation[4];          ///< Translation
    double _zoomFactor;             ///< Zoom factor
    int _currentFrameNumber = -1;   ///< Current frame number
    int _frameWidth = 0;            ///< Frame width
    int _frameHeight = 0;           ///< Frame height
    GLuint _currentTexture = 0;     ///< Current texture
    bool _invertColors = false;     ///< Negative display
    QGLBuffer   _quadVertexBuffer;  ///< Quad vertex buffer
    QGLBuffer   _quadTextureCoordsBuffer;  ///< Quad texture coordinates buffer
    QGLBuffer   _quadIndiceBuffer;  ///< Quad indice buffer
    QVector<GLuint> _indices;       ///< Vertice order
    QVector<QVector3D> _vertices;   ///< Current vertice order
    QVector<QVector2D> _textureCoords;   ///< Texture coordinates
    QGLShaderProgram _programShaderNegative;    ///< Negative glsl program
};

}
}
}

#endif
