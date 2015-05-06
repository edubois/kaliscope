#ifndef _PLAYEROPENGLWIDGET_HPP_
#define	_PLAYEROPENGLWIDGET_HPP_

#include <kali-core/typedefs.hpp>

#include <QtWidgets/QOpenGLWidget>

#include <boost/signals2.hpp>
#include <thread>

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

public:
    boost::signals2::signal<void( const std::size_t nFrame )> signalFrameDone;

protected:
    /**
     * @brief initialize opengl view
     */
    void initializeGL() override;

    /**
     * @brief resize opengl view
     */
    void resizeGL( int w, int h ) override;

    /**
     * @brief paint current video frame
     */
    void paintGL() override;

private:
    std::mutex _mutexDisplay;       ///< Locker
    int _currentFrameNumber = -1;   ///< Current frame number
    int _frameWidth = 0;            ///< Frame width
    int _frameHeight = 0;           ///< Frame height
    GLuint _currentTexture = 0;     ///< Current texture
};

}
}
}

#endif
