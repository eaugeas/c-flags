cmake_minimum_required(VERSION 3.16.3)
project(xroads)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options("$<$<CONFIG:RELEASE>:-std=c11;-Werror;-Wextra;-Wall;-pedantic;-O3;>")
add_compile_options("$<$<CONFIG:DEBUG>:-std=c11;-Werror;-Wextra;-Wall;-pedantic;-g;>")

add_subdirectory(deps)
list(APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_BINARY_DIR}/_deps/cmake-c-verify-src/cmake/")
message("CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")
include(c_verify)
include(c_verify_clang_format)
include(c_verify_clang_tidy)

enable_testing()
enable_c_verify()
enable_c_verify_clang_format()
enable_c_verify_clang_tidy()

add_subdirectory(source)
add_subdirectory(unit-tests)
