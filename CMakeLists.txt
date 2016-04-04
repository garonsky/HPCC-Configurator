PROJECT (hpccsystems-configurator)
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.2)

# Check if environment variable QTDIR is set.
# Extra security for windows environment as well.
IF( DEFINED ENV{QTDIR} )
    SET( CMAKE_PREFIX_PATH $ENV{QTDIR} ${CMAKE_PREFIX_PATH} )
    SET( Qt5_INCLUDE_DIRS ${CMAKE_PREFIX_PATH}/include )
    SET( Qt5_LIB_DIR ${CMAKE_PREFIX_PATH}/lib )
    MESSAGE( "-- Set CMAKE_PREFIX_PATH to ${CMAKE_PREFIX_PATH}" )
    MESSAGE( "-- CMAKE_MODULE_PATH is set to ${CMAKE_MODULE_PATH}" )
    MESSAGE( "-- Qt5_INCLUDE_DIRS is set to ${Qt5_INCLUDE_DIRS}" )
ELSEIF( CONFIGURATOR_STATIC_LIB )
    MESSAGE( FATAL_ERROR "ERROR: Environment variable QTDIR is not set. Please locate your Qt folder and set QTDIR.")
ENDIF()

SET( HPCC_CONFIGURATOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

SET( CONFIGURATOR_MODULE_PATH ${HPCC_CONFIGURATOR_SOURCE_DIR}/cmake_modules )
SET( QT_MODULE_PATH ${CMAKE_PREFIX_PATH}/lib/cmake)

SET( CMAKE_MODULE_PATH ${CONFIGURATOR_MODULE_PATH}  ${QT_MODULE_PATH} )
MESSAGE( "Set CMAKE_MODULE_PATH to ${CMAKE_MODULE_PATH}" )

IF( NOT BUILD_LEVEL )
	SET ( BUILD_LEVEL "COMMUNITY" )
ENDIF()

IF (NOT LICENSE_FILE)
    SET (LICENSE_FILE "LICENSE.txt")
ENDIF()

SET (HPCC_SOURCE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

IF ("${HPCC_SOURCE_DIR}" STREQUAL "")
  SET(HPCC_SOURCE_DIR ${HPCC_SOURCE_ROOT_DIR}/HPCC-Platform/)
  #MESSAGE( "Set HPCC_SOURCE_DIR to ${HPCC_SOURCE_DIR}" )
ENDIF()


INCLUDE (${HPCC_CONFIGURATOR_SOURCE_DIR}/version.cmake)
INCLUDE (${HPCC_SOURCE_DIR}/version.cmake)

INCLUDE (${HPCC_CONFIGURATOR_SOURCE_DIR}/cmake_modules/optionDefaults.cmake)
INCLUDE (${HPCC_CONFIGURATOR_SOURCE_DIR}/cmake_modules/configuratorCommonSetup.cmake)

IF( CONFIGURATOR_STATIC_LIB )
    INCLUDE (${QT_MODULE_PATH}/Qt5Widgets/Qt5WidgetsMacros.cmake)
ENDIF()

INCLUDE (${HPCC_CONFIGURATOR_SOURCE_DIR}/platform-version-prereq.cmake)

MESSAGE("-- Minimum HPCC Platform version required: ${HPCC_MAJOR_REQ}.${HPCC_MINOR_REQ}")
MESSAGE("--- Building against HPCC Platform version: ${HPCC_MAJOR}.${HPCC_MINOR}.${HPCC_POINT}-${HPCC_SEQUENCE}")

IF (${HPCC_MAJOR_REQ} GREATER ${HPCC_MAJOR} OR ( ${HPCC_MAJOR_REQ} EQUAL ${HPCC_MAJOR} AND ${HPCC_MINOR_REQ} GREATER ${HPCC_MINOR} ))
 MESSAGE(FATAL_ERROR "Project ${project} requires HPCC Platform version ${HPCC_MAJOR_REQ}.${HPCC_MINOR_REQ} or greater." )
ENDIF()

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fPIC -fmessage-length=0 -Wformat -Wformat-security -Wformat-nonliteral -pthread -Wuninitialized -std=c++11")

MESSAGE("*** STARTING BUILD OF HPCC PLATFORM ***")
MESSAGE (HPCC_SOURCE_DIR: ${HPCC_SOURCE_DIR})
ADD_SUBDIRECTORY(${HPCC_SOURCE_DIR} oss EXCLUDE_FROM_ALL)
INCLUDE(${HPCC_CONFIGURATOR_SOURCE_DIR}/platform-version-prereq.cmake)

IF ( CMAKE_SYSTEM_NAME MATCHES Linux )
    EXECUTE_PROCESS (
                COMMAND ${CMAKE_MODULE_PATH}/distrocheck.sh
                    OUTPUT_VARIABLE packageManagement
                        ERROR_VARIABLE  packageManagement
                )
    EXECUTE_PROCESS (
                COMMAND ${CMAKE_MODULE_PATH}/getpackagerevisionarch.sh
                    OUTPUT_VARIABLE packageRevisionArch
                        ERROR_VARIABLE  packageRevisionArch
                )
    EXECUTE_PROCESS (
                COMMAND ${CMAKE_MODULE_PATH}/getpackagerevisionarch.sh --noarch
                    OUTPUT_VARIABLE packageRevision
                        ERROR_VARIABLE  packageRevision
                )

    MESSAGE ( "-- Auto Detecting Packaging type")
    MESSAGE ( "-- distro uses ${packageManagement}, revision is ${packageRevisionArch}" )

    IF ( ${packageManagement} STREQUAL "DEB" )
        SET(CPACK_PACKAGE_FILE_NAME     "${CMAKE_PROJECT_NAME}-${version}-${stagever}${packageRevisionArch}")
        SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/lib)
    ELSEIF ( ${packageManagement} STREQUAL "RPM" )
        SET(CPACK_PACKAGE_FILE_NAME     "${CMAKE_PROJECT_NAME}-${version}-${stagever}.${packageRevisionArch}")
        SET(CMAKE_LIBRARY_PATH "/usr/lib64")
    ELSE()
        SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${version}-${stagever}${CPACK_SYSTEM_NAME}")
    ENDIF ()

ENDIF ( CMAKE_SYSTEM_NAME MATCHES Linux )

GET_PROPERTY(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS) 

IF ("${LIB64}" STREQUAL "TRUE") 
    SET(LIBSUFFIX 64) 
ELSE() 
    SET(LIBSUFFIX "") 
ENDIF() 

SET(INSTALL_LIB_DIR  lib${LIBSUFFIX} CACHE PATH "Installation directory for libraries") 
MARK_AS_ADVANCED(INSTALL_LIB_DIR)

HPCC_ADD_SUBDIRECTORY( configurator PLATFORM )
IF( CONFIGURATOR_STATIC_LIB )
    MESSAGE("-- Building QT Configurator Frontend " )
    HPCC_ADD_SUBDIRECTORY( configurator_ui PLATFORM )
    HPCC_ADD_SUBDIRECTORY( configurator_app PLATFORM )
ELSE()
    MESSAGE("-- Building CL Configurator " )
ENDIF()

GET_PROPERTY(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

SET ( CPACK_SOURCE_GENERATOR TGZ )

IF (NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.2")
    IF ( CMAKE_SYSTEM MATCHES Linux )
        IF ( ${packageManagement} STREQUAL "DEB" )
            IF ("${CMAKE_VERSION}" VERSION_EQUAL "2.8.2")
                MESSAGE("WARNING: CMAKE 2.8.2  would not build DEB package")
            ELSE ()
                SET ( CPACK_GENERATOR "${packageManagement}" )
                MESSAGE("-- Will build DEB package")
                ###
                ## CPack instruction required for Debian
                ###
                MESSAGE ("-- Packing BASH installation files")
            ENDIF ()

        ELSEIF ( ${packageManagement} STREQUAL "RPM" )
            SET ( CPACK_GENERATOR "${packageManagement}" )
            ###
            ## CPack instruction required for RPM
            ###
            MESSAGE("-- Will build RPM package")
            MESSAGE ("-- Packing BASH installation files")
        ELSE()
            MESSAGE("WARNING: Unsupported package ${packageManagement}.")
        ENDIF ()

    ENDIF ( CMAKE_SYSTEM MATCHES Linux )
    IF ( EXISTS ${HPCC_CONFIGURATOR_SOURCE_DIR}/cmake_modules/dependencies/${packageRevision}.cmake.in )
	CONFIGURE_FILE(${HPCC_CONFIGURATOR_SOURCE_DIR}/cmake_modules/dependencies/${packageRevision}.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/dependencies/${packageRevision}.cmake)
        CONFIGURE_FILE(${HPCC_SOURCE_DIR}/build-config.h.cmake "build-config.h")

	MESSAGE("-- Configuring ${packageRevision}.cmake.in --> ${packageRevision}.cmake")
        INCLUDE( ${CMAKE_CURRENT_BINARY_DIR}/dependencies/${packageRevision}.cmake )
    ELSE()
         MESSAGE("-- WARNING: DEPENDENCY FILE FOR ${packageRevision} NOT FOUND")
    ENDIF()

    IF ( UNIX )
     SET ( CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" )
    ENDIF()

    SET ( CPACK_MONOLITHIC_INSTALL TRUE )
		SET ( CPACK_PACKAGE_VENDOR "HPCC Systems" )
		INCLUDE ( CPack )
ELSE()
    MESSAGE("WARNING: CMAKE 2.8.1 or later required to create RPMs from this project")
ENDIF()


