cmake_minimum_required(VERSION 3.20)
project(uninstaller)

# ~~ uninstaller to cleanup any traces

set(LEAGUEPP_UNINSTALLER ${PROJECT_NAME} PARENT_SCOPE)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES} ${APP_RESOURCES_DIR}/resources.installer.rc)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${UIFRAMEWORK_INCLUDE}
    PRIVATE ${SHARED_HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${UIFRAMEWORK_LIB})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS")

target_compile_definitions(${PROJECT_NAME}
    PRIVATE APP_GUID="{${APP_GUID}}"
    PRIVATE BUILD_STRING="${BUILD_STRING}")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D "_MBCS")
endif()