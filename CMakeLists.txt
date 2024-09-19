cmake_minimum_required(VERSION 3.10)
project(algo)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB vec 
  math/vec/*.h
  math/vec/*.c 
)
file(GLOB mtx
  math/mtx/*.h
  math/mtx/*.c
)
file(GLOB sle
  math/sle/*.h
  math/sle/*.c 
)
file(GLOB ode
  math/ode/*.h
  math/ode/*.c 
)

add_executable(algo main.c
  math/def.h

  ${vec}
  ${mtx}
  ${sle}
  ${ode}
)

find_package(OpenMP REQUIRED)

target_include_directories(algo PUBLIC math/)
target_link_libraries(algo PUBLIC OpenMP::OpenMP_C)
target_link_libraries(algo PUBLIC m)

