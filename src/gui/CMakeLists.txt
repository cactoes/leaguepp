cmake_minimum_required(VERSION 3.20)
project(league++)

# ~~ gui interface

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${APP_RESOURCES_DIR}/resources.rc)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${SERVICE_INCLUDE}
    PRIVATE ${UIFRAMEWORK_INCLUDE}
    PRIVATE ${SHARED_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:CONSOLE")

target_link_libraries(${PROJECT_NAME}
    PRIVATE $<$<CONFIG:Debug>:${UIFRAMEWORK_LIB_DBG}> $<$<CONFIG:Release>:${UIFRAMEWORK_LIB_REL}>
    PRIVATE ${DEPENDENCY_DIR}/webview/WebView2/x64/WebView2LoaderStatic.lib
    PRIVATE Version.lib
    PRIVATE windowsapp.lib)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D "_MBCS")
endif()