#include "KaliscopeWin.hpp"

#include <mvp-player-core/trackTools.hpp>

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtGui/QDropEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDragMoveEvent>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>

#include <boost/format.hpp>

namespace kaliscope
{
namespace gui
{
namespace qt
{

KaliscopeWin::KaliscopeWin()
{
    widget.setupUi(this);

    mvpplayer::gui::qt::initDialog( *this );
    _viewer = new PlayerOpenGLWidget( this );
    _viewer->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    widget.layoutViewer->addWidget( _viewer );

    connect( _btnPlayPause, SIGNAL( toggled(bool) ), this, SLOT( slotViewHitPlayStopBtn() ) );
    connect( widget.btnConnect, SIGNAL( clicked(bool) ), this, SLOT( connectDisconnectClient( const bool ) ) );
    connect( widget.btnSettings, SIGNAL( released() ), this, SLOT( editSettings() ) );
    connect( widget.cbInvertColors, SIGNAL( toggled(bool) ), this, SLOT( invertDisplayColors( const bool ) ) );
}

KaliscopeWin::~KaliscopeWin()
{
    signalViewHitButton( "Stop", true );
    // Make sure all events are processed before we delete the view
    QApplication::processEvents();
}

boost::optional<boost::filesystem::path> KaliscopeWin::openFile( const std::string & title, const logic::EFileDialogMode mode, const std::string & extensions )
{
    QString result;
    QMetaObject::invokeMethod( this, "slotOpenFile", Qt::BlockingQueuedConnection, Q_RETURN_ARG( QString, result ), Q_ARG( QString, QString::fromStdString( title ) ), Q_ARG( QString, QString::fromStdString( extensions ) ), Q_ARG( logic::EFileDialogMode, mode ) );
    if ( !result.isEmpty() )
    {
        return boost::filesystem::path( result.toStdString() );
    }
    else
    {
        return boost::none;
    }
}

void KaliscopeWin::connectDisconnectClient( const bool start )
{
    if ( start )
    {
        signalViewConnect();
    }
    else
    {
        signalViewDisconnect();
    }
}

QString KaliscopeWin::slotOpenFile( const QString & title, const QString & extensions, const logic::EFileDialogMode mode )
{
    if ( mode == logic::eFileDialogModeOpen )
    {
        return QFileDialog::getOpenFileName( QApplication::activeWindow(), title, QDir::currentPath(), extensions );
    }
    else
    {
        return QFileDialog::getSaveFileName( QApplication::activeWindow(), title, QDir::currentPath(), extensions );
    }
}

void KaliscopeWin::slotDisplayError( const QString & msg )
{
    QMessageBox::critical( QApplication::activeWindow(), QObject::tr( "Error!" ), msg, QMessageBox::Ok | QMessageBox::Default);
}

void KaliscopeWin::setButtonChecked( const std::string & buttonName, const bool checked )
{
    auto it = _playerButtonsWidgets.find( buttonName );
    if ( it != _playerButtonsWidgets.end() )
    {
        it->second->setChecked( false );
    }
}

void KaliscopeWin::slotViewHitButton()
{
    QToolButton *button = qobject_cast<QToolButton*>( sender() );
    assert( button );
    if ( button->isCheckable() )
    {
        signalViewHitButton( button->objectName().toStdString(), button->isChecked() );
    }
    else
    {
        signalViewHitButton( button->objectName().toStdString(), true );
    }
}

void KaliscopeWin::changeTrackPosition( const int positionInPercent )
{
    signalViewHitTrackPosition( positionInPercent );
}

void KaliscopeWin::changeVolume( const int volume )
{
    signalViewSetVolume( volume / 100.0f );
}

void KaliscopeWin::slotViewHitPlayStopBtn()
{
    if ( _btnPlayPause->isChecked() == false )
    {
        signalViewHitButton( "Stop", true );
    }
    else
    {
        signalViewHitButton( "Play", true );
    }
}

void KaliscopeWin::playPlaylistItemAtIndex( const int playlistIndex )
{
//    signalViewHitPlaylistItem( playlistIndex );
}

void KaliscopeWin::openedPlaylist( const std::vector<m3uParser::PlaylistItem> & playlistItems )
{
    QStringList filenames;
    for( const m3uParser::PlaylistItem & item: playlistItems )
    {
        filenames.append( item.filename.string().c_str() );
    }
    QMetaObject::invokeMethod( this, "slotOpenedPlaylist", Qt::BlockingQueuedConnection, Q_ARG( QStringList, filenames ) );
}

void KaliscopeWin::dropEvent( QDropEvent *de )
{
    // Unpack dropped data and handle it
    if ( de->mimeData()->hasUrls() )
    {
        std::vector<boost::filesystem::path> fileItems;
        QList<QUrl> urlList = de->mimeData()->urls();
        fileItems.reserve( urlList.size() );
        for ( int i = 0; i < urlList.size(); ++i )
        {
            const QString url = urlList.at( i ).path();
            fileItems.push_back( url.toStdString() );
        }
        signalViewAppendTrackItems( fileItems );
        de->accept();
    }
}

void KaliscopeWin::invertDisplayColors( const bool active )
{
    _viewer->setInvertColors( active );
    _viewer->update();
}

void KaliscopeWin::editSettings()
{
    signalViewHitEditSettings();
}

void KaliscopeWin::dragEnterEvent( QDragEnterEvent *event )
{
    event->acceptProposedAction();
}

void KaliscopeWin::dragMoveEvent( QDragMoveEvent *event )
{
    event->acceptProposedAction();
}

void KaliscopeWin::dragLeaveEvent( QDragLeaveEvent *event )
{
    event->accept();
}

void KaliscopeWin::startStopServer( const bool start )
{
    // Unimplemented
}

void KaliscopeWin::slotSetIconStop()
{
    _btnPlayPause->setChecked( true );
}

void KaliscopeWin::slotSetIconPlay()
{
    _btnPlayPause->setChecked( false );
}

void KaliscopeWin::slotSetPlaylistItemIndex( const int row )
{
    // Unimplemented
}

void KaliscopeWin::slotOpenedPlaylist( const QStringList & filenames )
{
    // Unimplemented
}

void KaliscopeWin::slotClearedPlaylist()
{
    // Unimplemented
}

void KaliscopeWin::slotAddTrack( const QString & filename )
{
}

void KaliscopeWin::slotSetTrackLength( const std::size_t lengthInFrames )
{
    widget.sliderPosition->blockSignals( true ); // Don't forget to put this to avoid dead locks
    widget.sliderPosition->setValue( 0 );
    widget.sliderPosition->blockSignals( false );
    _currentTrackLength = lengthInFrames;
}

void KaliscopeWin::slotSetTrackPosition( const int positionInMS, const int trackLength )
{
    widget.sliderPosition->blockSignals( true ); // Don't forget to put this to avoid dead locks
    if ( trackLength != 0 )
    {
        widget.sliderPosition->setValue( 100 * positionInMS / trackLength );
    }
    else
    {
        widget.sliderPosition->setValue( 0 );
    }
    widget.sliderPosition->blockSignals( false );
}

void KaliscopeWin::slotSetVolume( const float volume )
{
    // Unimplemented
}

void KaliscopeWin::slotDisplayFrame( const std::size_t nFrame, const DefaultImageT & image )
{
    // Display frame number
    const double lengthInMS = _currentTrackLength * 1000.0 / 24.0;
    widget.lblTrackLength->setText( QString::fromStdString( ( boost::format( "%d / %d (%s)" ) % nFrame % _currentTrackLength % trackLengthToString( lengthInMS ) ).str() ) );
    // Display frame
    _viewer->setFrame( nFrame, image );
}

}
}
}
