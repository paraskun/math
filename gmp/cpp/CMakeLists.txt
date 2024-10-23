project(gmpcc CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -Wextra
                    -std=c++23")

set(SRC src/gmp_ucg.cc)
set(HDR include/gmp_ucg.hpp)

add_library(gmpcc ${SRC} ${HDR})

target_include_directories(gmpcc PUBLIC ./include/)
