file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    CLI11::CLI11
    ftxui::dom
    ftxui::screen
    ftxui::component
    ZLIB::ZLIB
    glaze::glaze
    OpenSSL::SSL
    OpenSSL::Crypto
    cpr::cpr
)

if(MSVC)
    set(OPTIONS_WARNINGS /W4)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(OPTIONS_OPTIMIZATION /O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OPTIONS_OPTIMIZATION /Zi)
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(OPTIONS_OPTIMIZATION /O2 /Zi)
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(OPTIONS_OPTIMIZATION /Os)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(OPTIONS_WARNINGS -Wall -Wpedantic)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(OPTIONS_OPTIMIZATION -O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OPTIONS_OPTIMIZATION -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(OPTIONS_OPTIMIZATION -O2 -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(OPTIONS_OPTIMIZATION -Os)
    endif()
endif()


message(STATUS "Build type:" ${CMAKE_BUILD_TYPE})
message(STATUS "Compiler options: ${OPTIONS_WARNINGS} ${OPTIONS_OPTIMIZATION}")

target_compile_options(${PROJECT_NAME} PUBLIC ${OPTIONS_WARNINGS} ${OPTIONS_OPTIMIZATION})

target_precompile_headers(
    ${PROJECT_NAME}
    PUBLIC
    <string>
    <string_view>
    <format>
    <regex>
    <optional>
    <utility>
    <functional>
    <memory>
    <mutex>
    <exception>
    <vector>
    <unordered_map>
    <algorithm>
    <chrono>
)

install(
    TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install DLLs on Windows
if(WIN32)
    install(
        TARGETS ${PROJECT_NAME}
        COMPONENT ${PROJECT_NAME}
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DIRECTORIES $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()