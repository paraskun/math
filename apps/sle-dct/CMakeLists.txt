set(MTX DMTX)

add_executable(sle-ddm src/sle_ddm.c)
add_executable(sle-hlb src/sle_hlb.c)
add_executable(sle-test src/sle_test.c)

target_link_libraries(sle-ddm PUBLIC math)
target_link_libraries(sle-hlb PUBLIC math)
target_link_libraries(sle-test PUBLIC math)

target_compile_definitions(sle-ddm PUBLIC ${MTX})
target_compile_definitions(sle-hlb PUBLIC ${MTX})
target_compile_definitions(sle-test PUBLIC ${MTX})
