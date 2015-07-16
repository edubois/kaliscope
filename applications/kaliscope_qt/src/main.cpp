#include "KaliscopeWin.hpp"

#include "settings/RecordingSettingsDialog.hpp"
#include <kali-core/stateMachineEvents.hpp>

#include <kali-core/VideoPlayer.hpp>
#include <kali-core/KaliscopeEngine.hpp>
#include <mvp-player-core/MVPPlayerEngine.hpp>
#include <mvp-player-core/MVPPlayerLogic.hpp>
#include <mvp-player-core/Settings.hpp>
#include <mvp-player-gui/playerBehavior.hpp>
#include <mvp-player-net/client/Client.hpp>
#include <mvp-player-net/server/Server.hpp>
#include <mvp-player-qtgui/MVPPlayerSettingsDialog.hpp>
#include <mvp-player-qtgui/resources.hpp>
#include <mvp-player-pluger/PluginLoader.hpp>
#include <mvp-player-net/client/Client.hpp>

#include <boost-adds/environment.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QStyleFactory>
#include <QtCore/QCoreApplication>

// Change your GUI here (qt)
namespace gui = kaliscope::gui::qt;
using Dialog = gui::KaliscopeWin;

void editSettings( QMainWindow *caller, mvpplayer::MVPPlayerEngine & m, mvpplayer::gui::IMVPPlayerDialog & v, mvpplayer::logic::MVPPlayerPresenter & p )
{
    mvpplayer::gui::qt::MVPPlayerSettingsDialog settingsDialog( m, v, p, caller );
    const int res = settingsDialog.exec();
    if ( !res )
    {
        mvpplayer::Settings::getInstance().write( QDir::homePath().toStdString() + "/" + mvpplayer::kDefaultSettingsFilename );
    }
}

bool editPipelineSettings( QMainWindow *caller, mvpplayer::Settings & settings )
{
    kaliscope::gui::qt::RecordingSettingsDialog dlg( caller );
    if ( dlg.exec() )
    {
        settings = dlg.pipelineSettings();
        dlg.pipelineSettings().write( QDir::homePath().toStdString() + "/" + kaliscope::gui::qt::kKaliscopeDefaultPipelineSettingsFilename );
        return true;
    }
    return false;
}

/**
 * @brief main goes here
 */
int main( int argc, char **argv )
{
    using namespace mvpplayer;
    {
        Settings::getInstance().read( QDir::homePath().toStdString() + "/" + kDefaultSettingsFilename );
        boost::optional<std::string> envStr = boost::get_env( plugins::kMVPPlayerPluginEnvKey );
        if ( envStr != boost::none && Settings::getInstance().has( "plugins", "pluginsPath" ) == false )
        {
            Settings::getInstance().set( "plugins", "pluginsPath", *envStr );
        }
    }

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

    QApplication app( argc, argv );
    Q_INIT_RESOURCE( shaders );
    initResources();

    int res = 0;
    try
    {
        using namespace tuttle::host;
        using namespace tuttle::common;

        Formatter::get();
        core().getPluginCache().addDirectoryToPath( Settings::getInstance().get<std::string>( "plugins", "pluginsPath" ) + "/OFX/" );
        core().preload();
    }
    catch( ... )
    {
        TUTTLE_LOG_CURRENT_EXCEPTION;
        const QString msg( boost::current_exception_diagnostic_information().c_str() );
        QMessageBox::critical( NULL, QObject::tr("Kaliscope"), QObject::tr("Failed to load OFX plugins (you need to install tuttleofx plugins)!\n") + msg );
        return -1;
    }

    Dialog dlg;
    // Core (model): a sound player engine
    kaliscope::KaliscopeEngine playerEngine( &kaliscope::VideoPlayer::getInstance() );

    // Network remote for synchronization (raspberry pi for example)
    mvpplayer::network::client::Client remote;

    // Presenter (presenter: logic-glu between model and view)
    mvpplayer::logic::MVPPlayerPresenter presenter;
    presenter.startStateMachine<mvpplayer::logic::PlayerStateMachine>();

    try
    {
        // Main dialog (view)
        QSystemTrayIcon trayIcon( QIcon( ":/mvpplayer/action/play.png" ) );
        trayIcon.setVisible( true );

        // Specific connections (be careful of the order here)

        // Display a message box on errors
        presenter.signalFailed.connect( boost::bind( &Dialog::displayError, &dlg, _1 ) );
        // When no file is provided and we hit play button, ask for a music file
        presenter.signalAskForFile.connect( boost::bind( &Dialog::openFile, &dlg, _1, _2, "Musics (*.dpx *.tif *.mov *.avi *.mp4);; All files (*.*)" ) );
        presenter.signalAskSettingsFor.connect( boost::bind( &editPipelineSettings, &dlg, _2 ) );

        // Setup Model View Presenter behavior (binds the whole thing)
        mvpplayer::gui::setupMainBehavior( playerEngine, dlg, presenter );
        // Settings editor binding
        dlg.signalViewHitEditSettings.connect( boost::bind( &editSettings, &dlg, boost::ref( playerEngine ), boost::ref( dlg ), boost::ref( presenter ) ) );

        // Transfer events received from the network to the presenter's state machine
        remote.signalEvent.connect( boost::bind( &mvpplayer::logic::MVPPlayerPresenter::processEvent, &presenter, _1 ) );

        // Network setup
        dlg.signalViewConnect.connect(
            [&remote, &dlg]()
            {
                static QString serverIP = "192.168.1.72";
                bool ok = false;
                serverIP = QInputDialog::getText( &dlg, QObject::tr("Connection"), QObject::tr("Kalisync server IP:"), QLineEdit::Normal, serverIP, &ok);
                if ( ok )
                {
                    remote.connect( serverIP.toStdString() );
                }
            }
        );
        dlg.signalViewDisconnect.connect( boost::bind( &mvpplayer::network::client::Client::disconnect, &remote ) );

        // Bind 'frame ready' signal to display function
        playerEngine.signalFrameReady.connect( boost::bind( &Dialog::displayFrame, &dlg, _1, _2 ) );
        // Used to signalize that a frame has been processed
        playerEngine.signalFrameReady.connect(
            [&remote]( const std::size_t, const kaliscope::DefaultImageT )
            {
                using EventT = mvpplayer::logic::EvCustomState;
                EventT event( kaliscope::kFrameCapturedCustomStateAction );
                remote.sendEvent( event );
            }
        );
        dlg.viewer()->signalFrameDone.connect( boost::bind( &kaliscope::KaliscopeEngine::frameProcessed, &playerEngine, _1 ) );

        // Load plugins
        mvpplayer::plugins::PluginLoader::getInstance().loadPlugins( playerEngine, dlg, presenter );

        dlg.showNormal();

        res = app.exec();
    }
    catch( ... )
    {
        TUTTLE_LOG_CURRENT_EXCEPTION;
        const QString msg( boost::current_exception_diagnostic_information().c_str() );
        QMessageBox::critical( NULL, QObject::tr("Kaliscope"), msg );
        res = -1;
    }

    // the following needs to be reviewed, it seems that boost::trackable has no effect on Qt objects
    dlg.viewer()->signalFrameDone.disconnect_all_slots();
    playerEngine.signalFrameReady.disconnect_all_slots();
    presenter.signalEvent.disconnect_all_slots();
    remote.signalEvent.disconnect_all_slots();
    app.processEvents();
    // Unload plugins
    mvpplayer::plugins::PluginLoader::getInstance().unloadPlugins();
    playerEngine.terminate();
    return res;
}
