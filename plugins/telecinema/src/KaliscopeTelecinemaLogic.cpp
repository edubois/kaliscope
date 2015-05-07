#include "KaliscopeTelecinemaLogic.hpp"

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
    TelecinemaPluginPresenter & plugPresenter = context< PlayerStateMachine >().presenter.presenterOfPlugin<TelecinemaPluginPresenter>( "recorderPlugin" );
    plugPresenter.signalRecord( mvpplayer::Settings() );
    return transit< Recording >();
}

/**
 * @brief reaction on stop track event
 */
sc::result Recording::react( const EvStop & ev )
{
    TelecinemaPluginPresenter & plugPresenter = context< PlayerStateMachine >().presenter.presenterOfPlugin<TelecinemaPluginPresenter>( "kaliscopeTelecinemaPlugin" );
    plugPresenter.signalStopRecord();
    return transit< Stopped >();
}

}
}
}
