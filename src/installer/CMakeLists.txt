cmake_minimum_required(VERSION 3.20)
project(installer)

# ~~ uninstaller to cleanup any traces

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES} ${APP_RESOURCES_DIR}/resources.installer.rc)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${UIFRAMEWORK_INCLUDE}
    PRIVATE ${SHARED_HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${UIFRAMEWORK_LIB})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/../../build/installer_linker/resource.res")

add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND node --no-deprecation installer_linker.js -c $(Configuration)
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../"
    DEPENDS ${LEAGUEPP_GUI} ${LEAGUEPP_SERVICE}
    COMMENT "Running pre-build script to link resources..."
)

add_custom_target(prebuild ALL DEPENDS ${RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.exe)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE APP_GUID="{${APP_GUID}}"
    PRIVATE BUILD_STRING="${BUILD_STRING}")

add_dependencies(${PROJECT_NAME} ${LEAGUEPP_GUI} ${LEAGUEPP_SERVICE})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D "_MBCS")
endif()