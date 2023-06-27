include(FetchContent)

FetchContent_Declare(
  cmake-c-verify
  GIT_REPOSITORY git@github.com:eaugeas/cmake-c-verify.git
)

FetchContent_MakeAvailable(cmake-c-verify)
list(APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_BINARY_DIR}/_deps/cmake-c-verify-src/cmake/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
