project(gmpcc CXX)

set(SRC src/gmp_ucg.cc)
set(HDR include/gmp_ucg.hpp)

add_library(gmpcc ${SRC} ${HDR})

target_include_directories(gmpcc PUBLIC ./include/)
