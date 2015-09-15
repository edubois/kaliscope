SET( MY_APP_NAME "Kaliscope" )

# external modules
include(UseMvpPlayerBoost)
include(FindFMod)
include(FindTuttleHost)
FIND_PACKAGE(Qt5 COMPONENTS Core Gui Widgets OpenGL Multimedia )
set(CMAKE_AUTOMOC ON)

FILE( GLOB_RECURSE KALISCOPE_SRCS "*.cpp" "*.hpp" "*.tcc" )

#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons
# Handle Mac OS X specific icons etc.
if(APPLE)
    # set how it shows up in the Info.plist file
    SET( MACOSX_BUNDLE_ICON_FILE )
    SET( MACOSX_BUNDLE_BUNDLE_VERSION "${KALISCOPEQT_VERSION}")
    SET( MACOSX_BUNDLE_INFO_STRING "${MY_APP_NAME} - version ${KALISCOPEQT_VERSION}" )

    # Change following line to point to actual icns file in bundle.
    SET( MACOSX_BUNDLE_GUI_IDENTIFIER "org.djarlabs.${MY_APP_NAME}" )
    SET( MACOSX_BUNDLE_NAME "${MY_APP_NAME}" )
    SET( prefix "${CMAKE_INSTALL_PREFIX}/${MACOSX_BUNDLE_NAME}.app/Contents" )
    SET( INSTALL_RUNTIME_DIR "${prefix}/MacOS" )
    SET( INSTALL_DATA_DIR "${prefix}/data" )
    SET( INSTALL_CMAKE_DIR "${prefix}/Resources" )
elseif(UNIX)
    #nothing
elseif(WIN32)
    LIST( APPEND KALISCOPE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/resources/${MY_APP_NAME}.rc )
endif()

ADD_EXECUTABLE( ${MY_APP_NAME} MACOSX_BUNDLE WIN32 ${KALISCOPE_SRCS} ${COPY_RESOURCES} )
TARGET_LINK_LIBRARIES( ${MY_APP_NAME} ${Boost_LIBRARIES} ${QT_LIBRARIES} ${TUTTLE_HOST_LIBRARIES} boostAdds-shared mvpPlayerCore-shared mvpPlayerNet-shared mvpPlayerFMod-shared mvpPlayerGui-shared mvpPlayerQtGui-shared kaliCore-shared  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL )

# Install stuff
SET( qt_plugin_dest_dir bin )
SET( qtconf_dest_dir bin )
SET( APPS "${CMAKE_INSTALL_PREFIX}/bin/${MY_APP_NAME}" )
IF( APPLE )
    SET( qt_plugin_dest_dir ${CMAKE_INSTALL_PREFIX}/${MY_APP_NAME}.app/Contents/PlugIns )
    SET( plugin_dest_dir ${INSTALL_CMAKE_DIR}/mvpPlayerPlugins )
    SET( qtconf_dest_dir ${INSTALL_CMAKE_DIR} )
    SET( APPS "${CMAKE_INSTALL_PREFIX}/${MY_APP_NAME}.app" )
ENDIF( APPLE )

IF( WIN32 )
    SET( APPS "${CMAKE_INSTALL_PREFIX}/bin/${MY_APP_NAME}.exe" )
ENDIF( WIN32 )

#--------------------------------------------------------------------------------
# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the 
#  IF(WIN32) conditional.
IF (WIN32)
    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)


#--------------------------------------------------------------------------------
# Install the ${MY_APP_NAME} application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
INSTALL( TARGETS ${MY_APP_NAME}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
)

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
IF(APPLE)
    GET_TARGET_PROPERTY( QT_PLUGINS_DIR Qt5::Core LOCATION_Release )
    GET_FILENAME_COMPONENT( QT_PLUGINS_DIR "${QT_PLUGINS_DIR}" DIRECTORY )
    GET_FILENAME_COMPONENT( QT_PLUGINS_DIR "${QT_PLUGINS_DIR}" DIRECTORY )
    GET_FILENAME_COMPONENT( QT_PLUGINS_DIR "${QT_PLUGINS_DIR}" DIRECTORY )
    SET( QT_PLUGINS_DIR "${QT_PLUGINS_DIR}/plugins" )

    INSTALL( DIRECTORY
        "${QT_PLUGINS_DIR}/platforms"
        "${QT_PLUGINS_DIR}/graphicssystems"
        DESTINATION ${qt_plugin_dest_dir}
        COMPONENT Runtime
        FILES_MATCHING
        REGEX "^.+[^0-9]+\\${CMAKE_SHARED_LIBRARY_SUFFIX}\$"
        PATTERN "*_debug${CMAKE_SHARED_LIBRARY_SUFFIX}" EXCLUDE
    )

    FILE( GLOB_RECURSE QTPLUGINS
          "${qt_plugin_dest_dir}/*${CMAKE_SHARED_LIBRARY_SUFFIX}" )

    INSTALL( DIRECTORY "${CMAKE_INSTALL_PREFIX}/mvpPlayerPlugins/"
        DESTINATION ${plugin_dest_dir}
        COMPONENT Runtime
        FILES_MATCHING
        PATTERN "*"
    )

    INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/src/resources/"
        DESTINATION ${INSTALL_CMAKE_DIR}
        COMPONENT Runtime
        FILES_MATCHING
        PATTERN "*.icns"
    )
    # set where in the bundle to put the icns file
    SET_SOURCE_FILES_PROPERTIES( "${MY_APP_NAME}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
    SET( MACOSX_BUNDLE_ICON_FILE "${MY_APP_NAME}.icns" )

    INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/ui"
        DESTINATION ${INSTALL_CMAKE_DIR}
        COMPONENT Runtime
        FILES_MATCHING
        PATTERN "*.ui"
    )

    INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/presets"
        DESTINATION ${INSTALL_CMAKE_DIR}
        COMPONENT Runtime
        FILES_MATCHING
        PATTERN "*.json"
    )

    FILE( GLOB_RECURSE KALISCOPE_PLUGINS
          "${plugin_dest_dir}/*${CMAKE_SHARED_LIBRARY_SUFFIX}" )

    FILE( GLOB_RECURSE OFX_PLUGINS
          "${plugin_dest_dir}/*.ofx" )

    # Add resources to application bundle target
    SET_TARGET_PROPERTIES(
     ${MY_APP_NAME}
     PROPERTIES
     RESOURCE "${COPY_RESOURCES}"
    )

    #--------------------------------------------------------------------------------
    # install a qt.conf file
    # this inserts some cmake code into the install script to write the file
    INSTALL( CODE "
        FILE(WRITE \"${qtconf_dest_dir}/qt.conf\" \"[Paths]\nPlugins = PlugIns\")
        " COMPONENT Runtime
    )

    SET( DIRS "${Boost_LIBRARY_DIR};${CMAKE_INSTALL_PREFIX}/lib;${FMOD_LIBRARY_DIR};${TUTTLE_HOST_LIBRARY_DIR}" )

    INSTALL(CODE "
        INCLUDE(BundleUtilities)
        fixup_bundle(${CMAKE_INSTALL_PREFIX}/${MY_APP_NAME}.app \"${QTPLUGINS};${KALISCOPE_PLUGINS};${OFX_PLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)
ENDIF(APPLE)
