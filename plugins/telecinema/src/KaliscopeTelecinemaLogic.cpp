#include "KaliscopeTelecinemaLogic.hpp"
#include "defines.hpp"

namespace kaliscope
{
namespace logic
{
namespace plugin
{
using namespace mvpplayer::logic;

/**
 * @brief reaction on record event
 */
sc::result Recording::react( const EvRecord & ev )
{
    TelecinemaPluginPresenter & plugPresenter = context< PlayerStateMachine >().presenter.presenterOfPlugin<TelecinemaPluginPresenter>( plugins::kMVPPlayerPluginName );
    plugPresenter.signalRecord( ev.recordingSettings() );
    return transit< Recording >();
}

/**
 * @brief reaction on stop track event
 */
sc::result Recording::react( const EvStop & ev )
{
    TelecinemaPluginPresenter & plugPresenter = context< PlayerStateMachine >().presenter.presenterOfPlugin<TelecinemaPluginPresenter>( plugins::kMVPPlayerPluginName );
    plugPresenter.signalStopRecord();
    return transit< Stopped >();
}

}
}
}
