cmake_minimum_required(VERSION 3.10)
project(algo)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)
set(C_STANDARD C23)

add_subdirectory(math)
add_subdirectory(apps)
