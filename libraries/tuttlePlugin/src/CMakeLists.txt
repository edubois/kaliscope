SET( TUTTLE_PLUGIN_SRC_PATH "${PROJECT_SOURCE_DIR}/src/tuttle" )
FILE( GLOB_RECURSE TUTTLE_PLUGIN_SRC_FILES "*.cpp" "*.hpp" "*.i" )

# external dependency
SET( Boost_USE_STATIC_LIBS OFF )
FIND_PACKAGE( Boost 1.58.0 
              COMPONENTS system log QUIET )

# Create 'tuttlePlugin' static lib
ADD_LIBRARY( tuttlePlugin-static STATIC ${TUTTLE_PLUGIN_SRC_FILES} )
SET_TARGET_PROPERTIES( tuttlePlugin-static PROPERTIES LINKER_LANGUAGE CXX )
SET_TARGET_PROPERTIES( tuttlePlugin-static PROPERTIES OUTPUT_NAME tuttlePlugin )
TARGET_INCLUDE_DIRECTORIES( tuttlePlugin-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../sequenceparser/src ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES( tuttlePlugin-static ${Boost_LIBRARIES} sequenceparser-static openfxHack-static terry )
IF(APPLE)
target_link_libraries( tuttlePlugin-static "-framework CoreFoundation" )
target_link_libraries( tuttlePlugin-static "-framework OpenGL" )
ENDIF(APPLE)

### Install tuttlePlugin libs and include
INSTALL(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	DESTINATION "include"
	FILES_MATCHING
	PATTERN "*.hpp"
	PATTERN "*.tcc"
	PATTERN "*.i"
)

INSTALL(TARGETS tuttlePlugin-static DESTINATION lib/ OPTIONAL)
