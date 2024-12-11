set(SRC
  src/const.h

  src/fem.c
  src/vtx.c
  src/fce.c
  src/hex.c
)

add_library(sse ${SRC})

target_link_libraries(sse PUBLIC mtx)
target_link_libraries(sse PUBLIC iss)
target_link_libraries(sse PUBLIC ext)

target_include_directories(sse PUBLIC include/)

add_executable(test_sse_case1 test/test_case1.c)
add_executable(test_sse_case2 test/test_case2.c)
add_executable(test_sse_case3 test/test_case3.c)
add_executable(test_sse_case4 test/test_case4.c)
add_executable(test_sse_case5 test/test_case5.c)

target_link_libraries(test_sse_case1 PUBLIC sse munit)
target_link_libraries(test_sse_case2 PUBLIC sse munit)
target_link_libraries(test_sse_case3 PUBLIC sse munit)
target_link_libraries(test_sse_case4 PUBLIC sse munit)
target_link_libraries(test_sse_case5 PUBLIC sse munit)

add_test(
  NAME test_sse_case1
  COMMAND test_sse_case1
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test
)

add_test(
  NAME test_sse_case2
  COMMAND test_sse_case2
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test
)

add_test(
  NAME test_sse_case3
  COMMAND test_sse_case3
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test
)

add_test(
  NAME test_sse_case4
  COMMAND test_sse_case4
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test
)

add_test(
  NAME test_sse_case5
  COMMAND test_sse_case5
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test
)
