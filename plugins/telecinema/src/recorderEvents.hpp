#ifndef _MVPPLUGIN_RECORDEREVENTS_HPP_
#define	_MVPPLUGIN_RECORDEREVENTS_HPP_

#include <mvp-player-core/MVPPlayerLogic.hpp>
#include <mvp-player-core/stateMachineEvents.hpp>

namespace kaliscope
{
namespace logic
{

namespace fs = boost::filesystem;
namespace sc = boost::statechart;

namespace plugin
{

/**
 * @brief event stop
 */
struct EvRecord : mvpplayer::IEvent, sc::event< EvRecord >
{
private:
    typedef EvRecord This;
public:
    
    EvRecord()
    {}

    EvRecord( const mvpplayer::Settings & settings )
    : _recordingSettings( settings )
    {
    }

    // This is needed to avoid a strange error on BOOST_CLASS_EXPORT_KEY
    static void operator delete( void *p, const std::size_t n )
    { ::operator delete(p); }

    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<IEvent>( *this );
    }

    /**
     * @brief don't dispatch this event thru the network
     */
    bool shallDispatch() const
    { return false; }

    /**
     * @brief process this event (needed to avoid dynamic_casts)
     * @param scheduler event scheduler
     * @param processor event processor
     */
    void processSelf( boost::statechart::fifo_scheduler<> & scheduler, boost::statechart::fifo_scheduler<>::processor_handle & processor )
    {
        scheduler.queue_event( processor, boost::intrusive_ptr< This >( this ) );
    }

    const mvpplayer::Settings & recordingSettings() const
    { return _recordingSettings; }

private:
    mvpplayer::Settings _recordingSettings;
};

template<class Archive>
void registerRecorderClassInArchive( Archive & ar )
{
    ar.template register_type< EvRecord >();
}

}

}
}

BOOST_CLASS_EXPORT_KEY( kaliscope::logic::plugin::EvRecord );

#endif
