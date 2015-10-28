/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

#ifndef _KALISCOPE_STATEMACHINEEVENTS_HPP_
#define	_KALISCOPE_STATEMACHINEEVENTS_HPP_

#include <mvp-player-core/IEvent.hpp>

#include <boost/statechart/event.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vector>

namespace kaliscope
{
    
static const std::string kFrameCapturedCustomStateAction( "kFrameCapturedCustomStateAction" );
static const std::string kCaptureStopCustomStateAction( "kCaptureStopCustomStateAction" );

}

#endif
