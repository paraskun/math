add_executable(prll-rcd src/rcd.c)
add_executable(prll-rcd-nb src/rcd_nb.c)

find_package(MPI REQUIRED)

target_link_libraries(prll-rcd PUBLIC ${MPI_C_LIBRARIES} math)
target_link_libraries(prll-rcd-nb PUBLIC ${MPI_C_LIBRARIES} math)
