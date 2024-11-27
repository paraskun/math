set(SRC
  src/fem.c
  src/vtx.c
  src/fce.c
  src/hex.c
)

add_library(fem_sse ${SRC})
target_link_libraries(fem_sse mtx iss)
target_include_directories(fem_sse PUBLIC include/)

add_executable(fem_sse_test test/fem_sse_test.cc)
target_link_libraries(fem_sse_test fem_sse GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(fem_sse_test
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test/
)
