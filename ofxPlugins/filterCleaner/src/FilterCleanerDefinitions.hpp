#ifndef _TUTTLE_PLUGIN_FILTERCLEANER_DEFINITIONS_HPP_
#define _TUTTLE_PLUGIN_FILTERCLEANER_DEFINITIONS_HPP_

#include <tuttle/plugin/global.hpp>

namespace tuttle {
namespace plugin {
namespace filterCleaner {

enum EParamAlgo
{
    eParamAlgoRGBReduction,
    eParamAlgoYUVReduction
};

static const std::string kParamHelpButton( "Help" );
static const std::string kParamHelpLabel( "Help" );
static const std::string kParamHelpString( "Remove orange filter that appears on film scans.\nYou must first analyze an image containing only the filter (or put the RGB value of the filter)." );

static const std::string kParamRedFilterColor( "Red filter color" );
static const std::string kParamRedFilterColorLabel( "Red filter color" );
static const unsigned short kParamDefaultRedFilterColor( 240 );

static const std::string kParamGreenFilterColor( "Green filter color" );
static const std::string kParamGreenFilterColorLabel( "Green filter color" );
static const unsigned short kParamDefaultGreenFilterColor( 140 );

static const std::string kParamBlueFilterColor( "Blue filter color" );
static const std::string kParamBlueFilterColorLabel( "Blue filter color" );
static const unsigned short kParamDefaultBlueFilterColor( 80 );

static const std::string kParamAlgorithm( "Algorithm" );
static const std::string kParamAlgorithmRGBReduction( "RGB reduction" );
static const std::string kParamAlgorithmYUVReduction( "YUV reduction" );

static const std::string kParamAnalyze( "Analyze" );
static const std::string kParamAnalyzeLabel( "Analyze filter" );

}
}
}

#endif
