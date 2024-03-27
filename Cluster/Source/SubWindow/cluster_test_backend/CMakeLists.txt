cmake_minimum_required(VERSION 3.1)
project(cluster_test_backend)
set(MODULE_NAME "cluster_test_backend")

ADD_DEFINITIONS(-std=c++14)


option(BUILD_SFC_FUEL "Build for SFCFuel" OFF)

if(BUILD_SFC_FUEL)
    message("*** turned ON BUILD_SFC_FUEL option ***")
    add_compile_definitions(BUILD_SFC_FUEL)
endif(BUILD_SFC_FUEL)

INCLUDE(FindPkgConfig)

# pkg_check_modules(HCOMMON REQUIRED hcommon)
# pkg_check_modules(HVEHICLE REQUIRED hvehicle)
# pkg_check_modules(SSFS.PRIVATE QUIET ssfs.private)

# if(NOT SSFS.PRIVATE_FOUND)
#     MESSAGE("-- ssfs.private not found. Using add_dependencies")
#     add_dependencies(${LIB_NAME} ssfs.private)
# endif(NOT SSFS.PRIVATE_FOUND)

include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

include(GNUInstallDirs)

add_executable(${MODULE_NAME} "")

target_include_directories(${MODULE_NAME} PUBLIC
    include
    PRIVATE
    # ${CMAKE_CURRENT_SOURCE_DIR}/../model/ssfs/include/public
    # ${CMAKE_CURRENT_SOURCE_DIR}/../model/ssfs/include/generated
    # ${CMAKE_CURRENT_SOURCE_DIR}/../model/ssfs/include/private
    # ${SSFS.PRIVATE_INCLUDE_DIRS}
)

FILE(GLOB SSFS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../model/ssfs/src/generated/*.cpp")

target_sources(${MODULE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ClusterTestBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AltonServiceRunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AltonClientRunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ClusterTestBackendLog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LogWatcher.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/SFCSignalDataGetter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TAVParser.cpp
    # ${HCOMMON_INCLUDE_DIRS}
    # ${HVEHICLE_INCLUDE_DIRS}
    # ${SSFS_SRCS}
)

target_link_libraries(${MODULE_NAME} PUBLIC
    pthread
    # ${SSFS.PRIVATE_LDFLAGS}
    # ${HCOMMON_LDFAGS}
    # ${HVEHICLE_LDFLAGS}
)

target_compile_features(${MODULE_NAME} PRIVATE cxx_auto_type)

install(TARGETS ${MODULE_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})