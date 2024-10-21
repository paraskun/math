set(MTX DMTX)

add_executable(ddm src/ddm.c)
add_executable(hlb src/hlb.c)
add_executable(test src/test.c)

target_link_libraries(ddm PUBLIC math)
target_link_libraries(hlb PUBLIC math)
target_link_libraries(test PUBLIC math)

target_compile_definitions(ddm PUBLIC ${MTX})
target_compile_definitions(hlb PUBLIC ${MTX})
target_compile_definitions(test PUBLIC ${MTX})
