add_executable(prll-vmlt src/vmlt.c)

find_package(MPI REQUIRED)

target_link_libraries(prll-vmlt PUBLIC ${MPI_C_LIBRARIES} math)
