cmake_minimum_required(VERSION 3.20)
project(league++)

# ~~ gui interface

set(LEAGUEPP_GUI ${PROJECT_NAME} PARENT_SCOPE)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${APP_RESOURCES_DIR}/resources.gui.rc)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${SERVICE_INCLUDE}
    PRIVATE ${UIFRAMEWORK_INCLUDE}
    PRIVATE ${SHARED_HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${UIFRAMEWORK_LIB})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:CONSOLE")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D "_MBCS")
endif()