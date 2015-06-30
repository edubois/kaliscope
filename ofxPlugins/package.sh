#!/bin/bash
install_name_tool -change $3libboost_filesystem.dylib $2libboost_filesystem.dylib $1
install_name_tool -change $3libboost_chrono.dylib $2libboost_chrono.dylib $1
install_name_tool -change $3libboost_date_time.dylib $2libboost_date_time.dylib $1
install_name_tool -change $3libboost_locale.dylib $2libboost_locale.dylib $1
install_name_tool -change $3libboost_log.dylib $2libboost_log.dylib $1
install_name_tool -change $3libboost_regex.dylib $2libboost_regex.dylib $1
install_name_tool -change $3libboost_system.dylib $2libboost_system.dylib $1
install_name_tool -change $3libboost_thread.dylib $2libboost_thread.dylib $1
install_name_tool -change $3libboost_serialization.dylib $2libboost_serialization.dylib $1
install_name_tool -change $3libboost_atomic.dylib $2libboost_atomic.dylib $1
install_name_tool -change $3libboost_program_options.dylib $2libboost_program_options.dylib $1
install_name_tool -change $3libboost_timer.dylib $2libboost_timer.dylib $1
install_name_tool -change $3tuttleHost.0.11.0.dylib $2libtuttleHost.dylib $1
install_name_tool -change $3tuttleHost.dylib $2libtuttleHost.dylib $1
install_name_tool -change $3libtuttleHost.dylib $2libtuttleHost.dylib $1
install_name_tool -change @rpath/libtuttleHost.0.11.0.dylib $2libtuttleHost.dylib $1

