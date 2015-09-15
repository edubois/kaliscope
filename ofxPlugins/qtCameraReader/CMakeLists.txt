# Macros used to create an openfx plugin with tuttle
include(TuttleMacros)

FIND_PACKAGE(Qt5 COMPONENTS Multimedia)

# Declare the plugin
tuttle_ofx_plugin_target(CameraReader)
TARGET_LINK_LIBRARIES( CameraReader ${QT_LIBRARIES} Qt5::Multimedia )
