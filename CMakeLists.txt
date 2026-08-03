cmake_minimum_required(VERSION 4.2)
project(realistic_map_generator C CXX OBJC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# # clang-tidy setup
# find_program(CLANG_TIDY_EXE
#     NAMES "clang-tidy"
#     HINTS
#         "$ENV{PROGRAMFILES}/LLVM/bin"
#         "$ENV{PROGRAMW6432}/LLVM/bin"
#     PATHS
#         "/opt/homebrew/opt/llvm/bin"
# )
#
# if(CLANG_TIDY_EXE)
#     message(STATUS "Found Clang-Tidy: ${CLANG_TIDY_EXE}")
#
#     # set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-warnings-as-errors=*")
#     set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
#
#    if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#         set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
#     endif()
# else()
#     message(WARNING "Clang-Tidy not found! Static analysis will not run.")
# endif()
#
#
# # cppcheck setup
# find_program(CPPCHECK_EXE NAMES "cppcheck"
#     HINTS
#         "$ENV{PROGRAMFILES}/Cppcheck"
#         "$ENV{PROGRAMW6432}/Cppcheck"
# )
#
# if(CPPCHECK_EXE)
#     message(STATUS "Found Cppcheck: ${CPPCHECK_EXE}")
#
#     set(CMAKE_CXX_CPPCHECK "${CPPCHECK_EXE};--enable=warning,style,performance;--inline-suppr;--std=c++20")
# else()
#     message(WARNING "Cppcheck not found! Static analysis will not run.")
# endif()

add_subdirectory(src)
add_subdirectory(external)
# add_subdirectory(tests)
