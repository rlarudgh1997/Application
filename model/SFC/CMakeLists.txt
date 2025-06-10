cmake_minimum_required(VERSION 3.15)
project(SFC_PlugIn)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      IF(EXISTS ${curdir}/${child}/CMakeLists.txt)
        LIST(APPEND dirlist ${child})
      ENDIF()
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(SUBDIR ${SUBDIRS})
  message("Including ${SUBDIR}")
  IF (SUBDIR MATCHES "build" OR SUBDIR MATCHES "include" OR SUBDIR MATCHES "compiled")
    CONTINUE()
  ENDIF()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
ENDFOREACH()

if(CCOS_SERVICE_ROOT_DIR)
  set(CONFIGURATION_INSTALL_DIR ${CCOS_SERVICE_ROOT_DIR}/altonservice/config/sfc)
else()
  set(CONFIGURATION_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/compiled/config)
endif()

FILE(GLOB SFC_CONFIGURATION_FILES "config/*.json")
install(
    FILES ${SFC_CONFIGURATION_FILES}
    DESTINATION ${CONFIGURATION_INSTALL_DIR}
)
