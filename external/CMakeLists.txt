# Adding Raylib (Inspiration: https://github.com/SasLuca/raylib-cmake-template/tree/master)
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE) # enable wayland on linux
set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE) # disable x11 on linux

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

# Adding FastNoise2
set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "" FORCE) # Don't build the GUI node editor
set(FASTNOISE2_TESTS OFF CACHE BOOL "" FORCE)     # Don't build tests
set(FASTNOISE2_EXAMPLES OFF CACHE BOOL "" FORCE)  # Don't build examples

FetchContent_Declare(
    FastNoise2
    GIT_REPOSITORY "https://github.com/Auburn/FastNoise2.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

# Suppress the __regcall warning from FastNoise2 on ARM
# Make this compiler specific so that we don't supply other compilers with wrong flags
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options(-Wno-ignored-attributes)
endif()

FetchContent_MakeAvailable(raylib FastNoise2)

add_library(Delaunator INTERFACE)

target_sources(Delaunator
    PUBLIC
        FILE_SET HEADERS
        FILES
            delaunator.hpp
)
