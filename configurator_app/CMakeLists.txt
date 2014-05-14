PROJECT ( configurator_app )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9)

QT5_WRAP_UI(MainWindow.ui ui_MainWindow.h)

SET( CMAKE_AUTOMOC ON )
SET( CMAKE_INCLUDE_CURRENT_DIR ON )

INCLUDE_DIRECTORIES(
        ${Qt5_INCLUDE_DIRS}
        ${Qt5_INCLUDE_DIRS}/QtGui
        ${Qt5_INCLUDE_DIRS}/QtWidgets
        ${Qt5_INCLUDE_DIRS}/QtCore
        ${Qt5_INCLUDE_DIRS}/QtQml
        ${HPCC_CONFIGURATOR_SOURCE_DIR}/configurator
        ${HPCC_CONFIGURATOR_SOURCE_DIR}/configurator_ui/
        ${HPCC_SOURCE_DIR}/system/jlib
        ${HPCC_SOURCE_DIR}/system/include
        ${HPCC_SOURCE_DIR}/deployment/deploy
 )

FIND_PACKAGE( Qt5Core )
FIND_PACKAGE( Qt5Widgets )
FIND_PACKAGE( Qt5OpenGL )
FIND_PACKAGE( Qt5Quick )
FIND_PACKAGE( Qt5Qml )

SET(    SRCS
        Main.cpp
        MainWindow.cpp
        Worker.cpp
        ${HPCC_CONFIGURATOR_SOURCE_DIR}/configurator_ui/AppData.cpp
)

HPCC_ADD_EXECUTABLE ( configurator_app ${SRCS} )

TARGET_LINK_LIBRARIES( configurator_app
    jlib
    Qt5::Qml
    Qt5::Widgets
    Qt5::Quick
    ${Qt5Core_LIBRARIES}
    configurator
    configurator_ui
)
