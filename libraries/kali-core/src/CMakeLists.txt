set(KALI_CORE_SRC_PATH "${PROJECT_SOURCE_DIR}/src/kali-core")
file(GLOB_RECURSE KALI_CORE_SRC_FILES "*.cpp" "*.hpp" )

# external dependency
SET( Boost_USE_STATIC_LIBS OFF )
FIND_PACKAGE( Boost 1.58.0 
              COMPONENTS system log filesystem QUIET )

FIND_PACKAGE( TuttleHost )

# Create 'kaliCore' static lib
add_library( kaliCore-static STATIC ${KALI_CORE_SRC_FILES} )
set_target_properties( kaliCore-static PROPERTIES LINKER_LANGUAGE CXX )
set_target_properties( kaliCore-static PROPERTIES OUTPUT_NAME kaliCore )
target_include_directories( kaliCore-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${TUTTLE_HOST_INCLUDE_DIRS} )
target_link_libraries( kaliCore-static ${Boost_LIBRARIES} ${TUTTLE_HOST_LIBRARIES} tuttlePlugin-static boostAdds-static mvpPlayerCore-static openfxHack-static )
# Create 'kaliCore' shared lib
add_library( kaliCore-shared SHARED ${KALI_CORE_SRC_FILES} )
set_target_properties( kaliCore-shared PROPERTIES LINKER_LANGUAGE CXX )
set_target_properties( kaliCore-shared PROPERTIES OUTPUT_NAME kaliCore )
set_target_properties( kaliCore-shared PROPERTIES SOVERSION ${KALI_CORE_VERSION_MAJOR} )
set_target_properties( kaliCore-shared PROPERTIES VERSION ${KALI_CORE_VERSION} )
target_include_directories( kaliCore-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${TUTTLE_HOST_INCLUDE_DIRS} )
target_link_libraries( kaliCore-shared ${Boost_LIBRARIES} ${TUTTLE_HOST_LIBRARIES} tuttlePlugin-static boostAdds-shared mvpPlayerCore-shared  openfxHack-static )
SET_TARGET_PROPERTIES( kaliCore-shared PROPERTIES MACOSX_BUNDLE TRUE )

### Install kaliCore libs and include
INSTALL(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	DESTINATION "include"
	FILES_MATCHING
	PATTERN "*.hpp"
	PATTERN "*.tcc"
)

INSTALL(TARGETS kaliCore-static DESTINATION lib/ OPTIONAL)
INSTALL(TARGETS kaliCore-shared DESTINATION lib/ OPTIONAL)
