cmake_minimum_required(VERSION 3.15)
project(ssfs)

if(NOT VEHICLE_CATEGORY)
    set(VEHICLE_CATEGORY PV)
endif()

if("${VEHICLE_CATEGORY}" STREQUAL "CV")
    set(SSFS_PV 0)
    set(SSFS_CONN 0)
    set(SSFS_CV 1)
    set(LIB_FOR_ALTON_ICV 1)
    set(LIB_FOR_ALTON_HEV 0)
    set(LIB_FOR_ALTON_PHEV 0)
    set(LIB_FOR_ALTON_EV 1)
    set(LIB_FOR_ALTON_FCEV 1)
    set(LIB_FOR_ALTON_EREV 0)
elseif("${VEHICLE_CATEGORY}" STREQUAL "CONN")
    set(SSFS_PV 0)
    set(SSFS_CONN 1)
    set(SSFS_CV 0)
    set(LIB_FOR_ALTON_ICV 1)
    set(LIB_FOR_ALTON_HEV 1)
    set(LIB_FOR_ALTON_PHEV 1)
    set(LIB_FOR_ALTON_EV 1)
    set(LIB_FOR_ALTON_FCEV 1)
    set(LIB_FOR_ALTON_EREV 1)
else()
    set(SSFS_PV 1)
    set(SSFS_CONN 0)
    set(SSFS_CV 0)
    set(LIB_FOR_ALTON_ICV 1)
    set(LIB_FOR_ALTON_HEV 1)
    set(LIB_FOR_ALTON_PHEV 1)
    set(LIB_FOR_ALTON_EV 1)
    set(LIB_FOR_ALTON_FCEV 1)
    set(LIB_FOR_ALTON_EREV 1)
endif()

if(NOT CCOS_LIB_VERSION)
    if(DEFINED ENV{CCOS_LIB_VERSION})
        set(CCOS_LIB_VERSION "$ENV{CCOS_LIB_VERSION}")
    else()
        message(FATAL_ERROR "add build option : \n -DCCOS_LIB_VERSION=X.X.X or export CCOS_LIB_VERSION=X.X.X")
    endif()
endif()

string(REPLACE "." ";" VERSION_LIST ${CCOS_LIB_VERSION})
list(GET VERSION_LIST 0 LIB_MAJOR_VERSION)
list(GET VERSION_LIST 1 LIB_MINOR_VERSION)
list(GET VERSION_LIST 2 LIB_PATCH_VERSION)
set(LIB_VERSION "${CCOS_LIB_VERSION}")
set(PROJECT_VERSION "${CCOS_LIB_VERSION}")

# set compile options
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -fPIC ${ALTERNATIVE_TARGET_CXX_FLAGS}")

FIND_PROGRAM(CCACHE_FOUND ccache)

IF(CCACHE_FOUND)
    SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
ENDIF(CCACHE_FOUND)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # for gcc
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--allow-shlib-undefined")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-O1,--hash-style=both")
else()
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,dynamic_lookup")
endif()

if(NOT CCOS_INC_DIR)
    set(CCOS_INC_DIR "${CMAKE_INSTALL_PREFIX}/include/ssfs")
endif()

if(NOT CCOS_SERVICE_DIR)
    set(CCOS_SERVICE_DIR "${CMAKE_INSTALL_PREFIX}/bin")
endif()

if(NOT CCOS_LIB_DIR)
    set(CCOS_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif()

if(NOT CCOS_SERVICE_ROOT_DIR)
    set(PLUGIN_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../SFC/compiled")
else()
    set(PLUGIN_INSTALL_PATH "${CCOS_SERVICE_ROOT_DIR}/altonservice/plugins")
endif()

include(FindPkgConfig)
pkg_check_modules(HCOMMON REQUIRED hcommon)
pkg_check_modules(HUTIL REQUIRED hutil)
pkg_check_modules(HVEHICLEHAL REQUIRED hvehiclehal)
pkg_check_modules(SSFS.PRIVATE QUIET ssfs.private)

if(NOT SSFS.PRIVATE_FOUND)
    MESSAGE("-- ssfs.private not found. Using add_dependencies")
    add_dependencies(${LIB_NAME} ssfs.private)
endif(NOT SSFS.PRIVATE_FOUND)

FILE(GLOB SSFS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/*.cpp")
FILE(GLOB SSFS_SFC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/SFC*.cpp")
FILE(GLOB SSFS_VSM_SYSTEM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.System*.cpp")
FILE(GLOB SSFS_VSM_SKEL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.*_SKEL.cpp")
FILE(GLOB SSFS_VSM_EV_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.*_EV.cpp")
FILE(GLOB SSFS_VSM_EREV_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.*_EREV.cpp")
FILE(GLOB SSFS_VSM_FCEV_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.*_FCEV.cpp")
FILE(GLOB SSFS_VSM_HEV_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.*_HEV.cpp")
FILE(GLOB SSFS_VSM_ICV_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.*_ICV.cpp")
FILE(GLOB SSFS_VSM_PHEV_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.*_PHEV.cpp")

SET(SSFS_VSM_COMMON_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/Vehicle.cpp
)
SET(SSFS_SFC_DATABASE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/SFCDataBase.cpp
)

SET(SSFS_INFO_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SSFSInfo.cpp)

set(SFC_HELPER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SFCHelper.cpp)

set(SFC_CONN_EVENTBOOK_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SFCEventBook_CONN.cpp)

set(SFC_CV_EVENTBOOK_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SFCEventBook_CV.cpp)

set(SFC_PV_EVENTBOOK_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SFCEventBook.cpp)

set(LIB_NAME_FOR_HMI ssfs)
add_library(${LIB_NAME_FOR_HMI} SHARED "")

# set(LIB_NAME_FOR_ALTON ssfs-alton) # deprecated
if(LIB_FOR_ALTON_ICV)
    set(LIB_NAME_FOR_ALTON_ICV ssfs-alton-ICV)
    add_library(${LIB_NAME_FOR_ALTON_ICV} SHARED "")
endif()

if(LIB_FOR_ALTON_HEV)
    set(LIB_NAME_FOR_ALTON_HEV ssfs-alton-HEV)
    add_library(${LIB_NAME_FOR_ALTON_HEV} SHARED "")
endif()

if(LIB_FOR_ALTON_PHEV)
    set(LIB_NAME_FOR_ALTON_PHEV ssfs-alton-PHEV)
    add_library(${LIB_NAME_FOR_ALTON_PHEV} SHARED "")
endif()

if(LIB_FOR_ALTON_EV)
    set(LIB_NAME_FOR_ALTON_EV ssfs-alton-EV)
    add_library(${LIB_NAME_FOR_ALTON_EV} SHARED "")
endif()

if(LIB_FOR_ALTON_FCEV)
    set(LIB_NAME_FOR_ALTON_FCEV ssfs-alton-FCEV)
    add_library(${LIB_NAME_FOR_ALTON_FCEV} SHARED "")
endif()

if(LIB_FOR_ALTON_EREV)
    set(LIB_NAME_FOR_ALTON_EREV ssfs-alton-EREV)
    add_library(${LIB_NAME_FOR_ALTON_EREV} SHARED "")
endif()

set(LIB_INCLUDE_DIRS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/public
    ${CMAKE_CURRENT_SOURCE_DIR}/include/generated
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private
    ${HCOMMON_INCLUDE_DIRS}
    ${HUTIL_INCLUDE_DIRS}
    ${HVEHICLEHAL_INCLUDE_DIRS}
    ${SSFS.PRIVATE_INCLUDE_DIRS})

set(LIB_LINK_LIBRARIES PRIVATE
    ${HCOMMON_LDFLAGS}
    ${HUTIL_LDFLAGS}
    ${SSFS.PRIVATE_LDFLAGS})

# ssfs for HMI
target_include_directories(${LIB_NAME_FOR_HMI} PUBLIC
    ${LIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/external)

if(SSFS_PV)
    message("-- SFCEventBook_PV Loading")
    target_sources(${LIB_NAME_FOR_HMI} PRIVATE
        ${SSFS_SFC_SRCS} ${SSFS_VSM_SYSTEM_SRCS} ${SSFS_VSM_COMMON_SRCS} ${SSFS_VSM_SKEL_SRCS}
        ${SFC_HELPER_SRC} ${SFC_PV_EVENTBOOK_SRC})
endif()

if(SSFS_CV)
    message("-- SFCEventBook_CV Loading")
    target_sources(${LIB_NAME_FOR_HMI} PRIVATE
        ${SSFS_SFC_SRCS} ${SSFS_VSM_SYSTEM_SRCS} ${SSFS_VSM_COMMON_SRCS} ${SSFS_VSM_SKEL_SRCS}
        ${SFC_HELPER_SRC} ${SFC_CV_EVENTBOOK_SRC})
endif()

if(SSFS_CONN)
    message("-- SFCEventBook_CONN Loading")
    target_sources(${LIB_NAME_FOR_HMI} PRIVATE
        ${SSFS_SFC_SRCS} ${SSFS_VSM_SYSTEM_SRCS} ${SSFS_VSM_COMMON_SRCS} ${SSFS_VSM_SKEL_SRCS}
        ${SFC_HELPER_SRC} ${SFC_CONN_EVENTBOOK_SRC})
endif()

target_link_libraries(${LIB_NAME_FOR_HMI} PRIVATE
    ${LIB_LINK_LIBRARIES})

install(
    TARGETS ${LIB_NAME_FOR_HMI}
    LIBRARY DESTINATION "${CCOS_LIB_DIR}"
)

set_target_properties(${LIB_NAME_FOR_HMI} PROPERTIES VERSION ${LIB_VERSION} SOVERSION ${LIB_MAJOR_VERSION})

# ----------------------------

# ssfs for alton - ev
if(LIB_FOR_ALTON_EV)
    target_compile_definitions(${LIB_NAME_FOR_ALTON_EV} PRIVATE -DSSFS_INTERNAL -DTAG_EV)

    target_include_directories(${LIB_NAME_FOR_ALTON_EV} PUBLIC
        ${LIB_INCLUDE_DIRS})

    target_sources(${LIB_NAME_FOR_ALTON_EV} PRIVATE
        ${SSFS_SFC_DATABASE_SRCS}
        ${SSFS_VSM_SYSTEM_SRCS}
        ${SSFS_VSM_COMMON_SRCS}
        ${SSFS_VSM_EV_SRCS}
        ${SSFS_VSM_SKEL_SRCS}
        ${SSFS_INFO_SRCS})

    target_link_libraries(${LIB_NAME_FOR_ALTON_EV} PRIVATE
        ${LIB_LINK_LIBRARIES}
    )

    install(
        TARGETS ${LIB_NAME_FOR_ALTON_EV}
        LIBRARY DESTINATION "${PLUGIN_INSTALL_PATH}"
    )
endif()

# ----------------------------

# ssfs for alton - fcev
if(LIB_FOR_ALTON_FCEV)
    target_compile_definitions(${LIB_NAME_FOR_ALTON_FCEV} PRIVATE -DSSFS_INTERNAL -DTAG_FCEV)

    target_include_directories(${LIB_NAME_FOR_ALTON_FCEV} PUBLIC
        ${LIB_INCLUDE_DIRS})

    target_sources(${LIB_NAME_FOR_ALTON_FCEV} PRIVATE
        ${SSFS_SFC_DATABASE_SRCS}
        ${SSFS_VSM_SYSTEM_SRCS}
        ${SSFS_VSM_COMMON_SRCS}
        ${SSFS_VSM_FCEV_SRCS}
        ${SSFS_VSM_SKEL_SRCS}
        ${SSFS_INFO_SRCS})

    target_link_libraries(${LIB_NAME_FOR_ALTON_FCEV} PRIVATE
        ${LIB_LINK_LIBRARIES}
    )

    install(
        TARGETS ${LIB_NAME_FOR_ALTON_FCEV}
        LIBRARY DESTINATION "${PLUGIN_INSTALL_PATH}"
    )
endif()

# ----------------------------

# ssfs for alton - erev
if(LIB_FOR_ALTON_EREV)
    target_compile_definitions(${LIB_NAME_FOR_ALTON_EREV} PRIVATE -DSSFS_INTERNAL -DTAG_EREV)

    target_include_directories(${LIB_NAME_FOR_ALTON_EREV} PUBLIC
        ${LIB_INCLUDE_DIRS})

    target_sources(${LIB_NAME_FOR_ALTON_EREV} PRIVATE
        ${SSFS_SFC_DATABASE_SRCS}
        ${SSFS_VSM_SYSTEM_SRCS}
        ${SSFS_VSM_COMMON_SRCS}
        ${SSFS_VSM_EREV_SRCS}
        ${SSFS_VSM_SKEL_SRCS}
        ${SSFS_INFO_SRCS})

    target_link_libraries(${LIB_NAME_FOR_ALTON_EREV} PRIVATE
        ${LIB_LINK_LIBRARIES}
    )

    install(
        TARGETS ${LIB_NAME_FOR_ALTON_EREV}
        LIBRARY DESTINATION "${PLUGIN_INSTALL_PATH}"
    )
endif()

# ----------------------------

# ssfs for alton - hev
if(LIB_FOR_ALTON_HEV)
    target_compile_definitions(${LIB_NAME_FOR_ALTON_HEV} PRIVATE -DSSFS_INTERNAL -DTAG_HEV)

    target_include_directories(${LIB_NAME_FOR_ALTON_HEV} PUBLIC
        ${LIB_INCLUDE_DIRS})

    target_sources(${LIB_NAME_FOR_ALTON_HEV} PRIVATE
        ${SSFS_SFC_DATABASE_SRCS}
        ${SSFS_VSM_SYSTEM_SRCS}
        ${SSFS_VSM_COMMON_SRCS}
        ${SSFS_VSM_HEV_SRCS}
        ${SSFS_VSM_SKEL_SRCS}
        ${SSFS_INFO_SRCS})

    target_link_libraries(${LIB_NAME_FOR_ALTON_HEV} PRIVATE
        ${LIB_LINK_LIBRARIES}
    )

    install(
        TARGETS ${LIB_NAME_FOR_ALTON_HEV}
        LIBRARY DESTINATION "${PLUGIN_INSTALL_PATH}"
    )
endif()

# ----------------------------

# ssfs for alton - icv
if(LIB_FOR_ALTON_ICV)
    target_compile_definitions(${LIB_NAME_FOR_ALTON_ICV} PRIVATE -DSSFS_INTERNAL -DTAG_ICV)

    target_include_directories(${LIB_NAME_FOR_ALTON_ICV} PUBLIC
        ${LIB_INCLUDE_DIRS})

    target_sources(${LIB_NAME_FOR_ALTON_ICV} PRIVATE
        ${SSFS_SFC_DATABASE_SRCS}
        ${SSFS_VSM_SYSTEM_SRCS}
        ${SSFS_VSM_COMMON_SRCS}
        ${SSFS_VSM_ICV_SRCS}
        ${SSFS_VSM_SKEL_SRCS}
        ${SSFS_INFO_SRCS})

    target_link_libraries(${LIB_NAME_FOR_ALTON_ICV} PRIVATE
        ${LIB_LINK_LIBRARIES}
    )

    install(
        TARGETS ${LIB_NAME_FOR_ALTON_ICV}
        LIBRARY DESTINATION "${PLUGIN_INSTALL_PATH}"
    )
endif()

# ----------------------------

# ssfs for alton - phev
if(LIB_FOR_ALTON_PHEV)
    target_compile_definitions(${LIB_NAME_FOR_ALTON_PHEV} PRIVATE -DSSFS_INTERNAL -DTAG_PHEV)

    target_include_directories(${LIB_NAME_FOR_ALTON_PHEV} PUBLIC
        ${LIB_INCLUDE_DIRS})

    target_sources(${LIB_NAME_FOR_ALTON_PHEV} PRIVATE
        ${SSFS_SFC_DATABASE_SRCS}
        ${SSFS_VSM_SYSTEM_SRCS}
        ${SSFS_VSM_COMMON_SRCS}
        ${SSFS_VSM_PHEV_SRCS}
        ${SSFS_VSM_SKEL_SRCS}
        ${SSFS_INFO_SRCS})

    target_link_libraries(${LIB_NAME_FOR_ALTON_PHEV} PRIVATE
        ${LIB_LINK_LIBRARIES}
    )

    install(
        TARGETS ${LIB_NAME_FOR_ALTON_PHEV}
        LIBRARY DESTINATION "${PLUGIN_INSTALL_PATH}"
    )
endif()

# ----------------------------
FILE(GLOB PUBLIC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/public/*")
FILE(GLOB GENERATED_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/generated/*")

install(
    FILES ${PUBLIC_HEADERS} ${GENERATED_HEADERS}
    DESTINATION ${CCOS_INC_DIR}
)

configure_file(ssfs.pc.cmakein ${CMAKE_CURRENT_BINARY_DIR}/ssfs.pc @ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/ssfs.pc
    DESTINATION "${CCOS_LIB_DIR}/pkgconfig"
)

# ssfs-alton.pc is deprecated
