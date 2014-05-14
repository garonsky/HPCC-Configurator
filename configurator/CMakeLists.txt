project ( configurator )

SET(  SRCS QMLMarkup.cpp QMLMarkup.hpp ConfiguratorMain.cpp ConfigFileComponentUtils.cpp ConfigFileUtils.cpp ConfigFileUtilsObservable.cpp
      ConfigFileUtilsObserver.cpp EnvironmentConfiguration.cpp WizardBase.cpp ConfigSchemaHelper.cpp
      SchemaAttributes.cpp SchemaComplexType.cpp SchemaSequence.cpp SchemaElement.cpp
      SchemaAppInfo.cpp SchemaAnnotation.cpp SchemaComplexContent.cpp SchemaComplexType.cpp SchemaDocumentation.cpp
      SchemaExtension.cpp SchemaChoice.cpp SchemaInclude.cpp SchemaSchema.cpp SchemaSimpleType.cpp SchemaRestriction.cpp NodeHandlerDocumentation.cpp DojoHelper.cpp
      SchemaEnumeration.cpp SchemaCommon.cpp ExceptionStrings.cpp SchemaEnumeration.cpp SchemaAttributeGroup.cpp DocumentationMarkup.hpp DojoJSMarkup.hpp
      SchemaCommon.hpp BuildSet.cpp ConfiguratorAPI.cpp SchemaMapManager.cpp EnvironmentModel.cpp)

  INCLUDE_DIRECTORIES(
        ${Qt5_INCLUDE_DIRS}
        ${Qt5_INCLUDE_DIRS}/QtGui
        ${Qt5_INCLUDE_DIRS}/QtCore
        ${Qt5_INCLUDE_DIRS}/QtNetwork
        ${Qt5_INCLUDE_DIRS}/QtQml
        ${Qt5_INCLUDE_DIRS}/QtQuick
        ${HPCC_SOURCE_DIR}/system/include
        ${HPCC_SOURCE_DIR}/system/jlib
        ${HPCC_SOURCE_DIR}/deployment/deploy
        ${CMAKE_PREFIX_PATH}/mkspecs/linux-g++
        ${CMAKE_CURRENT_SOURCE_DIR}/configurator_ui
        ${CMAKE_CURRENT_SOURCE_DIR}/configurator_app
)

IF( ${CONFIGURATOR_LIB} )
    MESSAGE( "Building configurator LIBRARY (.so) " )

    ADD_DEFINITIONS( -D_USRDLL -DCONFIGURATOR_STATIC_LIB)
    HPCC_ADD_LIBRARY( configurator SHARED ${SRCS} )
    TARGET_LINK_LIBRARIES( configurator jlib )

    INSTALL( TARGETS configurator LIBRARY DESTINATION ${LIB_DIR} )

ELSE()
    MESSAGE( "Building configurator EXECUTABLE" )

    ADD_DEFINITIONS( -D_CONSOLE )
    LINK_DIRECTORIES( ../configurator_ui Qt5_LIB_DIR )
    HPCC_ADD_EXECUTABLE( configurator  ${SRCS} )
    TARGET_LINK_LIBRARIES( configurator jlib configurator_ui)

    INSTALL( TARGETS configurator RUNTIME DESTINATION ${EXEC_DIR} )

ENDIF()
