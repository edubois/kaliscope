SET( TERRY_SRC_PATH "${PROJECT_SOURCE_DIR}/src/" )
FILE( GLOB_RECURSE TERRY_SRC_FILES "*.cpp" "*.hpp" )

# external dependency
SET( Boost_USE_STATIC_LIBS OFF )
FIND_PACKAGE( Boost 1.58.0 
              COMPONENTS system QUIET )

# Create 'terry' static lib (dummy)
add_library( terry STATIC ${TERRY_SRC_FILES} )
set_target_properties( terry PROPERTIES LINKER_LANGUAGE CXX )
set_target_properties( terry PROPERTIES OUTPUT_NAME terry )
TARGET_INCLUDE_DIRECTORIES( terry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/ ${Boost_INCLUDE_DIRS} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../tuttlePlugin/src/ )

### Install terry libs and include
INSTALL(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	DESTINATION "include"
	FILES_MATCHING
	PATTERN "*.hpp"
	PATTERN "*.tcc"
)

INSTALL( TARGETS terry DESTINATION lib/ OPTIONAL )
