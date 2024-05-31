cmake_minimum_required(VERSION 3.20)
project(leaguepp)

# ~~ c++ config stuff

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oxs")
set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++20")

# ~~ global cmake vars

set(DEPENDENCY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(APP_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(SHARED_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/shared)

# math(EXPR PLATFORM_BITS "8*${CMAKE_SIZEOF_VOID_P}")
# set(PLATFORM "x${PLATFORM_BITS}")

# ~~ projects

add_subdirectory(src/uiframework)
add_subdirectory(src/service)
add_subdirectory(src/gui)
add_subdirectory(src/installer)

# # ================
# # main
# # ================
# file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/client/*)
# add_executable(${PROJECT_NAME} ${SOURCE_FILES} resources.rc)
# target_link_libraries(${PROJECT_NAME} PRIVATE UIFramework)
# target_include_directories(${PROJECT_NAME} PRIVATE src/uiframework/include)
# target_include_directories(${PROJECT_NAME} PRIVATE src/shared)
# target_include_directories(${PROJECT_NAME} PRIVATE "lib/leaguepp-connector/lib/cpp-httplib")
# target_include_directories(${PROJECT_NAME} PRIVATE "lib/leaguepp-connector/lib/websocketpp")
# target_include_directories(${PROJECT_NAME} PRIVATE "lib/leaguepp-connector/lib/json/single_include")
# target_link_libraries(${PROJECT_NAME} PRIVATE
#     "$<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/src/chatproxy/target/debug/ChatProxyClient.dll.lib>"
#     "$<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/src/chatproxy/target/release/ChatProxyClient.dll.lib>"
# )

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "$<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/src/chatproxy/target/debug/ChatProxyClient.dll>"
#         "$<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/src/chatproxy/target/release/ChatProxyClient.dll>"
#     $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )

# add_custom_target(CopyDLLs ALL DEPENDS ${PROJECT_NAME})

# # ================
# # uiframework
# # ================
# file(GLOB_RECURSE UIFRAMEWORK_SOURCE_FILES CONFIGURE_DEPENDS src/uiframework/*)
# add_library(UIFramework SHARED ${UIFRAMEWORK_SOURCE_FILES})
# target_compile_definitions(UIFramework PRIVATE UIFRAMEWORK_EXPORTS)
# target_include_directories(UIFramework PRIVATE src/shared)
# target_include_directories(UIFramework PRIVATE "lib/leaguepp-connector/lib/json/single_include")

# # ================
# # library: WebView 2
# # ================
# set(WEBVIEW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib/webview)
# target_include_directories(UIFramework PRIVATE ${WEBVIEW_LIBRARY}/WebView2/include/)
# target_include_directories(UIFramework PRIVATE ${WEBVIEW_LIBRARY}/WIL/include/)
# target_link_libraries(UIFramework PRIVATE ${WEBVIEW_LIBRARY}/WebView2/x64/WebView2LoaderStatic.lib)
# target_link_libraries(UIFramework PRIVATE Version.lib)
# target_link_libraries(UIFramework PRIVATE windowsapp.lib)

# # ================
# # library: OpenSSL (v3)
# # note: required for cpp-httplib
# # ================
# set(OPENSSL_USE_STATIC_LIBS TRUE)
# find_package(OpenSSL REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)

# # ================
# # library: Boost
# # note: required for WebSocket++
# # ================
# set(Boost_USE_STATIC_LIBS TRUE)
# add_definitions("-D_WEBSOCKETPP_CPP11_STL_")
# find_package(Boost REQUIRED COMPONENTS system)
# target_include_directories(${PROJECT_NAME} PRIVATE Boost::headers)
# target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)

# # ================
# # library: league++connector
# # ================
# add_subdirectory(lib/leaguepp-connector)
# target_link_libraries(${PROJECT_NAME} PRIVATE leaguepp_connector)

# # add the license to the build
# # add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy LICENSE $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# if(MSVC)
#     target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D "_MBCS")
#     target_compile_options(UIFramework PRIVATE /W4 /D "_MBCS")
#     set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oxs")
#     set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++20")
# else()
#     message(FATAL_ERROR "Unsupported compiler")
# endif()

# # ================
# # WiX
# # ================
# install(TARGETS ${PROJECT_NAME} DESTINATION .)
# install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/build/Release/UIFramework.dll" DESTINATION .)
# install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/build/Release/ChatProxyClient.dll" DESTINATION .)
# install(FILES "LICENSE" DESTINATION .)
# install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/Release/view" DESTINATION .)

# set(CPACK_GENERATOR "WIX")
# set(CPACK_PACKAGE_NAME "league++")
# set(CPACK_PACKAGE_VENDOR "NERV")
# set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "league++ client")
# set(CPACK_PACKAGE_VERSION "1.0.0")
# set(CPACK_WIX_UPGRADE_GUID "F9857995-5C72-42AF-BC0F-641B565E5DB1")

# set(CPACK_WIX_LICENSE_RTF "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.rtf")
# set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/disc_icon_2.ico")
# set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "league++")

# include(CPack)