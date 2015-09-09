cmake_minimum_required(VERSION 2.8.11)

project( OpenfxHack CXX )

# Set sequenceParser versions
set( OPENFXHACK_VERSION_MAJOR "1" )
set( OPENFXHACK_VERSION_MINOR "0" )
set( OPENFXHACK_VERSION_MICRO "0" )
set( OPENFXHACK_VERSION ${OPENFXHACK_VERSION_MAJOR}.${OPENFXHACK_VERSION_MINOR}.${OPENFXHACK_VERSION_MICRO} )

# Define sequenceParser versions
add_definitions( -DOPENFXHACK_VERSION_MAJOR=${OPENFXHACK_VERSION_MAJOR} )
add_definitions( -DOPENFXHACK_VERSION_MINOR=${OPENFXHACK_VERSION_MINOR} )
add_definitions( -DOPENFXHACK_VERSION_MICRO=${OPENFXHACK_VERSION_MICRO} )
add_definitions( -DOPENFXHACK_STATIC_BUILD )

# Diplay commands being ran by CMake
set( CMAKE_VERBOSE_MAKEFILE OFF )

# CPP flags on debug / release mode
if( MSVC )
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fPIC -pg -g")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC -O3")
endif()

set(OPENFXHACK_SRC_PATH "${PROJECT_SOURCE_DIR}/Support/Library")
file(GLOB_RECURSE OPENFXHACK_SRC_FILES "*.cpp" "*.hpp" )

# external dependency
SET( Boost_USE_STATIC_LIBS OFF )
FIND_PACKAGE( Boost 1.58.0 
              COMPONENTS system QUIET )

# Create 'openfxHack' static lib
add_library( openfxHack-static STATIC ${OPENFXHACK_SRC_FILES} )
set_target_properties( openfxHack-static PROPERTIES LINKER_LANGUAGE CXX )
set_target_properties( openfxHack-static PROPERTIES OUTPUT_NAME openfxHack )
target_include_directories( openfxHack-static PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../tuttlePlugin/src/ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Support/include ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} )
target_link_libraries( openfxHack-static ${Boost_LIBRARIES} )

### Install openfxHack libs and include
install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	DESTINATION "include"
	FILES_MATCHING
	PATTERN "*.hpp"
	PATTERN "*.tcc"
)

INSTALL( TARGETS openfxHack-static DESTINATION lib/ OPTIONAL )
