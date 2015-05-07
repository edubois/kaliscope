#ifndef _PLUG_KALISCOPETELECINEMAPLUGIN_HPP_
#define	_PLUG_KALISCOPETELECINEMAPLUGIN_HPP_

#include "KaliscopeTelecinemaLogic.hpp"

#include <mvp-player-pluger/IMVPPlugin.hpp>

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#ifndef Q_PLUGIN_METADATA
#define Q_PLUGIN_METADATA(...)
#endif

Q_DECLARE_INTERFACE( mvpplayer::plugins::IMVPPlugin, "org.djarlabs.mvpplayer.plugins.IMVPPlugin/1.0" );

namespace kaliscope
{
namespace plugins
{
static const std::string kMVPPlayerPluginName( "kaliscopeTelecinemaPlugin" );
static const std::string kRecordAction( "kRecordAction::kaliscope" );

class KaliscopeTelecinemaPlugin : public mvpplayer::plugins::IMVPPlugin
{
    Q_OBJECT
    Q_INTERFACES( mvpplayer::plugins::IMVPPlugin )
    Q_PLUGIN_METADATA( IID "org.djarlabs.kaliscope.plugins.kaliscopeTelecinemaPlugin" FILE "kaliscopeTelecinemaPlugin.json" )
public:
    KaliscopeTelecinemaPlugin()
    : IMVPPlugin( kMVPPlayerPluginName )
    {}
    
    virtual void setup( mvpplayer::MVPPlayerEngine & model, mvpplayer::gui::IMVPPlayerDialog & view, mvpplayer::logic::MVPPlayerPresenter & presenter );

    /**
     * Triggered when the user click on the record button
     * @param record shall we record or stop recording ?
     */
    void recordClicked( const bool record );
    
    /**
     * @brief Needed to enter into the Recording state
     */
    boost::statechart::detail::reaction_result recordTransition( const std::string & action, mvpplayer::logic::Stopped & state );

    /**
     * Triggered when we want to start the recording using given settings
     * @param settings recording settings
     */
    void record( const mvpplayer::Settings & settings );

    /**
     * @brief Triggered when the capture is done
     */
    void playTrack();

private:
    logic::plugin::TelecinemaPluginPresenter _plugPresenter;
};

}
}

#endif
