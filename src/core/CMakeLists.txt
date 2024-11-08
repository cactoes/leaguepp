cmake_minimum_required(VERSION 3.20)
project(core)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES} resources/resources.gui.rc)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${ALL_LIB_INCLUDES}
    PRIVATE include
    PRIVATE ${LIB_DIR}/leaguepp-connector/lib/json/single_include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:${LIB_DIR}/reflection/lib/reflection_core.debug.lib>
        $<$<CONFIG:Release>:${LIB_DIR}/reflection/lib/reflection_core.release.lib>)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_compile_options(${PROJECT_NAME} PRIVATE ${CUSTOM_COMPILE_OPTIONS})

# ~~ openssl3
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)

# ~~ boost
set(Boost_USE_STATIC_LIBS ON)
add_definitions("-D_WEBSOCKETPP_CPP11_STL_")
find_package(Boost REQUIRED COMPONENTS system)
target_include_directories(${PROJECT_NAME} PRIVATE Boost::headers)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)

# ~~ league++connector
add_subdirectory(${LIB_DIR}/leaguepp-connector ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lppc)
target_link_libraries(${PROJECT_NAME} PRIVATE leaguepp_connector)