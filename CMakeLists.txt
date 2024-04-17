cmake_minimum_required(VERSION 3.20)
project(leaguepp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ================
# main
# ================
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} resources.rc)
target_link_libraries(${PROJECT_NAME} PRIVATE UIFramework)
target_include_directories(${PROJECT_NAME} PRIVATE uiframework/include)
target_include_directories(${PROJECT_NAME} PRIVATE shared)

# ================
# uiframework
# ================
file(GLOB_RECURSE UIFRAMEWORK_SOURCE_FILES CONFIGURE_DEPENDS uiframework/*)
add_library(UIFramework SHARED ${UIFRAMEWORK_SOURCE_FILES})
target_compile_definitions(UIFramework PRIVATE UIFRAMEWORK_EXPORTS)
target_include_directories(UIFramework PRIVATE shared)

# ================
# library: WebView 2
# ================
set(WEBVIEW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib/webview)
target_include_directories(UIFramework PRIVATE ${WEBVIEW_LIBRARY}/WebView2/include/)
target_include_directories(UIFramework PRIVATE ${WEBVIEW_LIBRARY}/WIL/include/)
target_link_libraries(UIFramework PRIVATE ${WEBVIEW_LIBRARY}/WebView2/x64/WebView2LoaderStatic.lib)
target_link_libraries(UIFramework PRIVATE Version.lib)
target_link_libraries(UIFramework PRIVATE windowsapp.lib)
# if (NOT WEB_VIEW_STATIC)
#     set(WEBVIEW_DLL ${WINDOWS_EXTERNAL}/WebView2/x64/WebView2Loader.dll.lib)
#     set(WEBVIEW_DLL ${WINDOWS_EXTERNAL}/WebView2/x64/WebView2Loader.dll)
#     add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different ${WEBVIEW_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>
#         COMMENT "Copying WebView2Loader.dll"
#     )
# endif()

# ================
# library: OpenSSL (v3)
# note: required for cpp-httplib
# ================
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)
target_link_libraries(UIFramework PRIVATE OpenSSL::SSL)

# ================
# library: Boost
# note: required for WebSocket++
# ================
set(Boost_USE_STATIC_LIBS TRUE)
add_definitions("-D_WEBSOCKETPP_CPP11_STL_")
find_package(Boost REQUIRED COMPONENTS system)
target_include_directories(${PROJECT_NAME} PRIVATE Boost::headers)
target_include_directories(UIFramework PRIVATE Boost::headers)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)
target_link_libraries(UIFramework PRIVATE Boost::system)

# ================
# library: league++connector
# ================
add_subdirectory(lib/leaguepp-connector)
target_link_libraries(${PROJECT_NAME} PRIVATE leaguepp_connector)
# for json
target_link_libraries(UIFramework PRIVATE leaguepp_connector)

# add the license to the build
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy LICENSE $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D "_MBCS")
    target_compile_options(UIFramework PRIVATE /W4 /D "_MBCS")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oxs")
    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++20")
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()