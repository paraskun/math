project(gmpcc-exe CXX)

add_executable(gmpcc-exe gmp.cc)

target_link_libraries(gmpcc-exe gmpcc)
