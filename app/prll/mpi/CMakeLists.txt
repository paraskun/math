add_executable(hw hw.c)
add_executable(nrm nrm.c)
add_executable(nrm-nb nrm_nb.c)
add_executable(mlt mlt.c)

find_package(MPI REQUIRED)

target_link_libraries(hw PRIVATE vec ${MPI_C_LIBRARIES})
target_link_libraries(nrm PRIVATE vec ${MPI_C_LIBRARIES})
target_link_libraries(nrm-nb PRIVATE vec ${MPI_C_LIBRARIES})
target_link_libraries(mlt PRIVATE vec ${MPI_C_LIBRARIES})

