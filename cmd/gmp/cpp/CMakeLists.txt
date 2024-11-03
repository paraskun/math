project(ucg-spp CXX)

add_executable(ucg-spp ucg_spp.cc)
add_executable(ucg-add ucg_add.cc)

target_link_libraries(ucg-spp gmpcc)
target_link_libraries(ucg-add gmpcc)
