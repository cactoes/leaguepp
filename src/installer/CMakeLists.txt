cmake_minimum_required(VERSION 3.20)
project(installer)

# ~~ uninstaller to cleanup any traces

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${APP_RESOURCES_DIR}/resources.installer.rc) # ${APP_RESOURCES_DIR}/installer.dbg.rc

target_include_directories(${PROJECT_NAME}
    PRIVATE ${UIFRAMEWORK_INCLUDE}
    PRIVATE ${SHARED_HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE shlwapi.lib
    PRIVATE Shell32.lib
    PRIVATE ${UIFRAMEWORK_LIB})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:CONSOLE")

add_dependencies(${PROJECT_NAME} ${LEAGUEPP_GUI} ${LEAGUEPP_SERVICE})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D "_MBCS")
endif()