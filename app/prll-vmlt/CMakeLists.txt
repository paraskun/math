add_executable(main src/main.c)
add_executable(gen src/gen.c)

find_package(MPI REQUIRED)

target_link_libraries(main PUBLIC ${MPI_C_LIBRARIES} math)
target_link_libraries(gen PUBLIC math)
