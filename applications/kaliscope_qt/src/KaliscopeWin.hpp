#ifndef _KALISCOPEWIN_HPP_
#define	_KALISCOPEWIN_HPP_

#include "ui_KaliscopeWin.hpp"

#include "opengl/PlayerOpenGLWidget.hpp"

#include <kali-core/typedefs.hpp>
#include <mvp-player-core/MVPPlayerPresenter.hpp>
#include <mvp-player-gui/IMVPPlayerDialog.hpp>
#include <mvp-player-qtgui/dialogInit.hpp>

namespace kaliscope
{
namespace gui
{
namespace qt
{

using namespace mvpplayer;

class KaliscopeWin : public QMainWindow, public mvpplayer::gui::IMVPPlayerDialog
{
    Q_OBJECT
private:
    typedef KaliscopeWin This;
protected:
    friend void mvpplayer::gui::qt::initDialog<This>( This & dlg );
public:
    KaliscopeWin();
    virtual ~KaliscopeWin();

    void displayError( const std::string & msg )
    { QMetaObject::invokeMethod( this, "slotDisplayError", Qt::BlockingQueuedConnection, Q_ARG( QString, msg.c_str() ) ); }

    boost::optional<boost::filesystem::path> openFile( const std::string & title, const logic::EFileDialogMode mode, const std::string & extensions );

    /**
     * In the following sections, we use invokeMethod because of asynchronous
     * calls that might come from other threads.
     */
    inline void setIconStop() override
    { QMetaObject::invokeMethod( this, "slotSetIconStop", Qt::BlockingQueuedConnection ); }

    inline void setIconPlay() override
    { QMetaObject::invokeMethod( this, "slotSetIconPlay", Qt::BlockingQueuedConnection ); }

    inline void setCurrentTrack( const boost::filesystem::path & filename ) override
    { QMetaObject::invokeMethod( widget.lblCurrentTrack, "setText", Qt::BlockingQueuedConnection, Q_ARG( QString, QString::fromStdString( filename.filename().string() ) ) ); }

    inline void setPlaylistItemIndex( const int row ) override
    { QMetaObject::invokeMethod( this, "slotSetPlaylistItemIndex", Qt::BlockingQueuedConnection, Q_ARG( int, row ) ); }

    inline void setTrackPosition( const std::size_t positionInMS, const std::size_t trackLength ) override
    { QMetaObject::invokeMethod( this, "slotSetTrackPosition", Qt::BlockingQueuedConnection, Q_ARG( int, positionInMS ), Q_ARG( int, trackLength ) ); }

    inline void setTrackLength( const std::size_t lengthInMS ) override
    { QMetaObject::invokeMethod( this, "slotSetTrackLength", Qt::BlockingQueuedConnection, Q_ARG( std::size_t, lengthInMS ) ); }

    void openedPlaylist( const std::vector<m3uParser::PlaylistItem> & playlistItems ) override;

    inline void clearedPlaylist() override
    { QMetaObject::invokeMethod( this, "slotClearedPlaylist", Qt::BlockingQueuedConnection ); }

    inline void addTrack( const boost::filesystem::path & filename ) override
    { QMetaObject::invokeMethod( this, "slotAddTrack", Qt::BlockingQueuedConnection, Q_ARG( QString, QString::fromStdString( filename.filename().string() ) ) ); }

    inline void setVolume( const float volume ) override
    { QMetaObject::invokeMethod( this, "slotSetVolume", Qt::BlockingQueuedConnection, Q_ARG( float, volume ) ); }

    inline void displayFrame( const std::size_t nFrame, const DefaultImageT & image )
    { QMetaObject::invokeMethod( this, "slotDisplayFrame", Qt::BlockingQueuedConnection, Q_ARG( std::size_t, nFrame ), Q_ARG( DefaultImageT, image ) ); }

    /**
     * @brief get the opengl video viewer
     * @return the viewer (pointer)
     */
    inline PlayerOpenGLWidget *viewer()
    { return _viewer; }

private:
    void setButtonChecked( const std::string & buttonName, const bool checked );
    void dropEvent( QDropEvent *de );
    void dragEnterEvent( QDragEnterEvent *event );
    void dragMoveEvent( QDragMoveEvent *event );
    void dragLeaveEvent( QDragLeaveEvent *event );

protected Q_SLOTS:
    QString slotOpenFile( const QString & title, const QString & extensions, const logic::EFileDialogMode mode );
    void slotDisplayError( const QString & msg );
    void slotViewHitButton();
    void changeTrackPosition( const int positionInPercent );
    void changeVolume( const int volume );
    void invertDisplayColors( const bool );
    void editSettings();
    void startStopServer( const bool start = true );
    void playPlaylistItemAtIndex( const int playlistIndex );
    void slotViewHitPlayStopBtn();
    void slotSetPlaylistItemIndex( const int row );
    void slotSetIconStop();
    void slotSetIconPlay();
    void slotOpenedPlaylist( const QStringList & filenames );
    void slotClearedPlaylist();
    void slotAddTrack( const QString & filename );
    void slotSetTrackPosition( const int positionInMS, const int trackLength );
    void slotSetTrackLength( const std::size_t lengthInMS );
    void slotSetVolume( const float volume );
    void slotDisplayFrame( const std::size_t nFrame, const DefaultImageT & image );

protected:
    std::size_t _currentTrackLength = 0;
    QToolButton *_btnPlayPause;
    QToolButton *_btnPrevious;
    QToolButton *_btnNext;
    std::map<std::string, QToolButton*> _playerButtonsWidgets;

private:
    Ui::KaliscopeWin widget;
    PlayerOpenGLWidget *_viewer;
};

}
}
}

#endif
