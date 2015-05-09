#include "KaliscopeWin.hpp"

#include "settings/RecordingSettingsDialog.hpp"

#include <kali-core/VideoPlayer.hpp>
#include <kali-core/KaliscopeEngine.hpp>
#include <mvp-player-core/MVPPlayerEngine.hpp>
#include <mvp-player-core/MVPPlayerLogic.hpp>
#include <mvp-player-core/SoundPlayer.hpp>
#include <mvp-player-core/Settings.hpp>
#include <mvp-player-gui/playerBehavior.hpp>
#include <mvp-player-net/client/Client.hpp>
#include <mvp-player-net/server/Server.hpp>
#include <mvp-player-qtgui/MVPPlayerSettingsDialog.hpp>
#include <mvp-player-qtgui/resources.hpp>
#include <mvp-player-pluger/PluginLoader.hpp>

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
    /*
    mvpplayer::gui::qt::MVPPlayerSettingsDialog settingsDialog( m, v, p, caller );
    const int res = settingsDialog.exec();
    if ( !res )
    {
        mvpplayer::Settings::getInstance().write( QDir::homePath().toStdString() + "/" + mvpplayer::kDefaultSettingsFilename );
    }
    */
    kaliscope::gui::qt::RecordingSettingsDialog dlg( caller );
    dlg.exec();
}

/**
 * @brief main goes here
 */
int main( int argc, char **argv )
{
    {
        using namespace mvpplayer;
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

    initResources();

    Dialog dlg;

    using namespace tuttle::host;
    using namespace tuttle::common;

    Formatter::get();
    core().preload();


    // Core (model): a sound player engine
    kaliscope::KaliscopeEngine playerEngine( &kaliscope::VideoPlayer::getInstance() );

    // Presenter (presenter: logic-glu between model and view)
    mvpplayer::logic::MVPPlayerPresenter presenter;
    presenter.startStateMachine<mvpplayer::logic::PlayerStateMachine>();

    // Main dialog (view)
    QSystemTrayIcon trayIcon( QIcon( ":/mvpplayer/action/play.png" ) );
    trayIcon.setVisible( true );

    // Specific connections (be careful of the order here)

    // Display a message box on errors
    presenter.signalFailed.connect( boost::bind( &Dialog::displayError, &dlg, _1 ) );
    // When no file is provided and we hit play button, ask for a music file
    presenter.signalAskForFile.connect( boost::bind( &Dialog::openFile, &dlg, _1, _2, "Musics (*.dpx *.tif *.mov *.avi *.mp4);; All files (*.*)" ) );

    // Setup Model View Presenter behavior (binds the whole thing)
    mvpplayer::gui::setupMainBehavior( playerEngine, dlg, presenter );
    // Settings editor binding
    dlg.signalViewHitEditSettings.connect( boost::bind( &editSettings, &dlg, boost::ref( playerEngine ), boost::ref( dlg ), boost::ref( presenter ) ) );
    
    // Bind 'frame ready' signal to display function
    playerEngine.signalFrameReady.connect( boost::bind( &Dialog::displayFrame, &dlg, _1, _2 ) );
    dlg.viewer()->signalFrameDone.connect( boost::bind( &kaliscope::KaliscopeEngine::frameProcessed, &playerEngine, _1 ) );

    // Load plugins
    mvpplayer::plugins::PluginLoader::getInstance().loadPlugins( playerEngine, dlg, presenter );

    dlg.showNormal();

    return app.exec();
}
