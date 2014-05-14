CMAKE_MINIMUM_REQUIRED( VERSION 2.8.11 )

PROJECT(configurator_ui)

SET( SRCS AppData.cpp ConfiguratorUI.cpp )

# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)

INCLUDE_DIRECTORIES(
        ${Qt5_INCLUDE_DIRS}
        ${Qt5_INCLUDE_DIRS}/QtCore
        ${Qt5_INCLUDE_DIRS}/QtGui
        ${Qt5_INCLUDE_DIRS}/QtQml
        ${HPCC_CONFIGURATOR_SOURCE_DIR}/configurator
        ${HPCC_CONFIGURATOR_SOURCE_DIR}/configurator_app
)

ADD_DEFINITIONS( -D_USRDLL -DCONFIGURATOR_STATIC_LIB )
HPCC_ADD_LIBRARY ( configurator_ui  ${SRCS} )

