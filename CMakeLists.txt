cmake_minimum_required(VERSION 3.21)
project(math C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "-Wall -Werror -Wextra -Wpedantic -std=c23")
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -Wpedantic -std=c++23")

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

find_package(OpenMP)

if (OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

  add_compile_definitions(OMP_THREADS_NUM=2)
endif()

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(vec)
add_subdirectory(gmp)
add_subdirectory(pde)
