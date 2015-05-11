#ifndef _KALISCOPETELECINEMALOGIC_HPP_
#define	_KALISCOPETELECINEMALOGIC_HPP_

#include "recorderEvents.hpp"

#include <mvp-player-core/Settings.hpp>
#include <mvp-player-core/MVPPlayerLogic.hpp>
#include <mvp-player-core/stateMachineEvents.hpp>

#include <iostream>

namespace kaliscope
{
namespace logic
{

namespace fs = boost::filesystem;
namespace sc = boost::statechart;

namespace plugin
{

struct TelecinemaPluginPresenter : mvpplayer::logic::IPluginPresenter
{
    boost::signals2::signal<void( const mvpplayer::Settings & settings )> signalRecord;
    boost::signals2::signal<void()> signalStopRecord;
    boost::signals2::signal<void()> signalNextFrame;
};

/**
 * @brief Recording state
 */
struct Recording : sc::simple_state< Recording, mvpplayer::logic::Active >
{
    typedef boost::mpl::list<
      sc::custom_reaction< EvRecord >,
      sc::custom_reaction< mvpplayer::logic::EvStop >,
      sc::custom_reaction< mvpplayer::logic::EvNextTrack >
    > reactions;

    /**
     * @brief reaction on record event
     */
    sc::result react( const EvRecord & ev );

    /**
     * @brief reaction on stop track event
     */
    sc::result react( const mvpplayer::logic::EvStop & ev );

    /**
     * @brief reaction on next frame event
     */
    sc::result react( const mvpplayer::logic::EvNextTrack & ev );

};

}

}
}

#endif
