#ifndef _TUTTLE_PLUGIN_DCRAWREADER_DEFINITIONS_HPP_
#define _TUTTLE_PLUGIN_DCRAWREADER_DEFINITIONS_HPP_

#include <tuttle/plugin/global.hpp>


namespace tuttle {
namespace plugin {
namespace dcrawReader {

static const std::string kParamInterpolationQuality( "Interpolation quality" );
static const std::string kParamInterpolationQualityLinear( "0 (Linear interpolation)" );
static const std::string kParamInterpolationQualityVNG( "1 (Interpolation using a Threshold-based variable number of gradients)" );
static const std::string kParamInterpolationQualityPPG( "2 (Patterned Pixel Grouping Interpolation by Alain Desbiolles)" );
static const std::string kParamAlgorithmYUVReductionAHD( "3 (Adaptive Homogeneity-Directed interpolation)" );

}
}
}

#endif
