#include "KaliscopeTelecinemaPlugin.hpp"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <boost/filesystem/path.hpp>

namespace kaliscope
{
namespace plugins
{
using namespace mvpplayer::logic;

void KaliscopeTelecinemaPlugin::setup( mvpplayer::MVPPlayerEngine & model, mvpplayer::gui::IMVPPlayerDialog & view, mvpplayer::logic::MVPPlayerPresenter & presenter )
{
    // Call base class' setup
    IMVPPlugin::setup( model, view, presenter );
    // Intercept record button click
    view.signalViewHitButton.connect( [this, &presenter]( const std::string & commandName, const bool record ) { if ( commandName == "Record" ) this->recordClicked( record ); } );
    presenter.askStoppedStateExternalTransition.connect( boost::bind( &KaliscopeTelecinemaPlugin::recordTransition, this, _1, _2 ) );

    presenter.registerPluginPresenter( kMVPPlayerPluginName, _plugPresenter );
    _plugPresenter.signalRecord.connect( boost::bind( &KaliscopeTelecinemaPlugin::record, this, _1 ) );
    _plugPresenter.signalStopRecord.connect( boost::bind( &MVPPlayerPresenter::processCommandActive, &presenter, std::string( "Record" ), false ) );
}

/**
 * Triggered when the user click on the record button
 */
void KaliscopeTelecinemaPlugin::recordClicked( const bool activated )
{
    if ( activated )
    {
        // Stop current action
        _presenter->processStop();

        // Ask for recording settings
        mvpplayer::Settings recordingSettings;
        const auto settingsSet = _presenter->signalAskSettingsFor( "Record", recordingSettings );
        // Cancelled ?
        if ( settingsSet == boost::none || settingsSet == false )
        {
            return;
        }

        // Queue custom event to enter the right plugin recording state
        {
            using EventT = EvCustomState;
            EventT *event = new EventT( kRecordAction );
            _presenter->signalEvent( *event );
            _presenter->processEvent( *event );
        }
        // Queue record event
        {
            using EventT = logic::plugin::EvRecord;
            EventT *event = new EventT( recordingSettings );
            _presenter->signalEvent( *event );
            _presenter->processEvent( *event );
        }
    }
    else
    {
        // Queue stop event
        _presenter->processStop();
    }
}

/**
 * @brief Needed to enter into the Recording state
 */
boost::statechart::detail::reaction_result KaliscopeTelecinemaPlugin::recordTransition( const std::string & action, Stopped & state )
{
    if ( action == kRecordAction )
    {
        const auto result = state.transit<logic::plugin::Recording>();
        // The following is needed to silent assert in boost
        const auto consume = sc::detail::result_utility::get_result( result );
        return boost::statechart::detail::consumed;
    }
    else
    {
        return boost::statechart::detail::no_reaction;
    }
}

/**
 * Triggered when we want to start the recording using given settings
 * @param settings recording settings
 */
void KaliscopeTelecinemaPlugin::record( const mvpplayer::Settings & settings )
{
    // Recording goes here:
    std::cout << "recording" << std::endl;
}

void KaliscopeTelecinemaPlugin::playTrack()
{
    // Queue stop event
    _presenter->processStop();
    _presenter->processPlay( boost::none );
}


}
}

#if (QT_VERSION < 0x050000)
Q_EXPORT_PLUGIN2( kaliscope::plugins::KaliscopeTelecinemaPlugin, KaliscopeTelecinemaPlugin )
#endif // (QT_VERSION >= 0x050000)
