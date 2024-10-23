add_executable(jac jac.c)
add_executable(rlx rlx.c)
add_executable(cds-ddm cds_ddm.c)

target_link_libraries(jac iss)
target_link_libraries(rlx iss)
target_link_libraries(cds-ddm mtx)
