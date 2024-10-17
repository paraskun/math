add_executable(prll-hw src/hw.c)

find_package(MPI REQUIRED)

target_link_libraries(prll-hw PUBLIC ${MPI_C_LIBRARIES})
