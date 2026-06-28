cmake_minimum_required(VERSION 4.2)
project(realistic_map_generator CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(tests)
