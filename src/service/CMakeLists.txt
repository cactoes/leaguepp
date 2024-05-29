cmake_minimum_required(VERSION 3.20)
project(lppservice)

# ~~ main logic

set(SERVICE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
file(GLOB_RECURSE INCLUDE_FILES CONFIGURE_DEPENDS include/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} ${APP_RESOURCES_DIR}/resources.service.rc)

target_include_directories(${PROJECT_NAME} PRIVATE include ${SHARED_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:CONSOLE")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D "_MBCS")
endif()