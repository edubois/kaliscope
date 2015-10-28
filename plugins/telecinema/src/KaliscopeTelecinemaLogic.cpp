/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

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

/**
 * @brief reaction on next frame event
 */
sc::result Recording::react( const EvNextTrack & )
{
    TelecinemaPluginPresenter & plugPresenter = context< PlayerStateMachine >().presenter.presenterOfPlugin<TelecinemaPluginPresenter>( plugins::kMVPPlayerPluginName );
    plugPresenter.signalNextFrame();
    return transit< Recording >();
}

/**
 * @brief reaction on next frame event
 */
sc::result Recording::react( const EvPlay & )
{
    TelecinemaPluginPresenter & plugPresenter = context< PlayerStateMachine >().presenter.presenterOfPlugin<TelecinemaPluginPresenter>( plugins::kMVPPlayerPluginName );
    plugPresenter.signalContinuousRecording();
    plugPresenter.signalNextFrame();
    return transit< Recording >();
}

}
}
}
