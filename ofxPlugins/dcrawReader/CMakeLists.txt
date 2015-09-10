# Macros used to create an openfx plugin with tuttle
include(TuttleMacros)

ADD_DEFINITIONS( -DNO_JASPER )
ADD_DEFINITIONS( -DNO_LCMS )
ADD_DEFINITIONS( -DNO_JPEG )

# Declare the plugin
tuttle_ofx_plugin_target(DCRawReader)
