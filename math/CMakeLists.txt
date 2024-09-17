cmake_minimum_required(VERSION 3.10)
project(math)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB vec 
  vec/*.h
  vec/*.c 
)
file(GLOB mtx
  mtx/*.h
  mtx/*.c
)
file(GLOB sle
  sle/*.h
  sle/*.c 
)
file(GLOB ode
  ode/*.h
  ode/*.c 
)

add_executable(math 
  main.c
  def.h
  ${vec}
  ${mtx}
  ${sle}
  ${ode}
)

find_package(OpenMP REQUIRED)

target_include_directories(math PUBLIC . mtx/ ode/ sle/ vec/)
target_link_libraries(math PUBLIC OpenMP::OpenMP_C)
target_link_libraries(math PUBLIC m)

