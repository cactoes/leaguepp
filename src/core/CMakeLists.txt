cmake_minimum_required(VERSION 3.20)
project(core)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} resources/resources.gui.rc) # WIN32

target_include_directories(${PROJECT_NAME}
    PRIVATE ${ALL_LIB_INCLUDES}
    PRIVATE include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:${LIB_DIR}/reflection/lib/reflection_core.debug.lib>
        $<$<CONFIG:Release>:${LIB_DIR}/reflection/lib/reflection_core.release.lib>

    PRIVATE
        $<$<CONFIG:Debug>:${LIB_DIR}/leaguepp_connector/lib/connector.debug.lib>
        $<$<CONFIG:Release>:${LIB_DIR}/leaguepp_connector/lib/connector.release.lib>)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_compile_options(${PROJECT_NAME} PRIVATE ${CUSTOM_COMPILE_OPTIONS})