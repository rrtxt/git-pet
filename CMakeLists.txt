cmake_minimum_required(VERSION 3.20)

project(git-pet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v7.0.1
)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(ftxui tomlplusplus stb)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGIT2 REQUIRED libgit2)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(git-pet ${SOURCES})

target_include_directories(git-pet PRIVATE
    include
    ${LIBGIT2_INCLUDE_DIRS}
)

target_include_directories(git-pet PRIVATE
    include
)

target_link_libraries(git-pet PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
    tomlplusplus::tomlplusplus
    stb
    ${LIBGIT2_LIBRARIES}
)
