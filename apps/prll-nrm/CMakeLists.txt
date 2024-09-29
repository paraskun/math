add_executable(prll-nrm src/nrm.c)
add_executable(prll-nrm-nb src/nrm_nb.c)

find_package(MPI REQUIRED)

target_link_libraries(prll-nrm PUBLIC ${MPI_C_LIBRARIES} math)
target_link_libraries(prll-nrm-nb PUBLIC ${MPI_C_LIBRARIES} math)
