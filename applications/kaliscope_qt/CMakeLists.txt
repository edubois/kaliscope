cmake_minimum_required(VERSION 2.8.11)

project( Kaliscope CXX )

# Set sequenceParser versions
set( KALISCOPEQT_VERSION_MAJOR "1" )
set( KALISCOPEQT_VERSION_MINOR "0" )
set( KALISCOPEQT_VERSION_MICRO "0" )
set( KALISCOPEQT_VERSION ${KALISCOPEQT_VERSION_MAJOR}.${KALISCOPEQT_VERSION_MINOR}.${KALISCOPEQT_VERSION_MICRO} )

# Define sequenceParser versions
add_definitions( -DKALISCOPEQT_VERSION_MAJOR=${KALISCOPEQT_VERSION_MAJOR} )
add_definitions( -DKALISCOPEQT_VERSION_MINOR=${KALISCOPEQT_VERSION_MINOR} )
add_definitions( -DKALISCOPEQT_VERSION_MICRO=${KALISCOPEQT_VERSION_MICRO} )
add_definitions( -DKALISCOPEQT_STATIC_BUILD )

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

add_subdirectory(src)
