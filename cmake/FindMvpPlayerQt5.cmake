# Qt for the whole mvp-player project
find_path( QT5_INCLUDE_DIR GL/glew.h )
find_library( QT5_LIBRARY NAMES QT5 QtWidgets QtGui QtCore )

SET(QT5_INCLUDE_DIRS ${QT5_INCLUDE_DIR})
SET(QT5_LIBRARIES ${QT5_LIBRARY})

INCLUDE( FindPackageHandleStandardArgs )
find_package_handle_standard_args( QT5 QT5_INCLUDE_DIR QT5_LIBRARY )

mark_as_advanced( QT5_INCLUDE_DIR QT5_LIBRARY )
