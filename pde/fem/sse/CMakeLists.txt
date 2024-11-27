set(SRC
  src/fem.c
  src/vtx.c
  src/fce.c
  src/hex.c
)

add_library(sse ${SRC})

target_link_libraries(sse PUBLIC mtx)
target_link_libraries(sse PUBLIC iss)

target_include_directories(sse PUBLIC include/)

add_executable(sse_test test/sse_test.cc)
target_link_libraries(sse_test sse GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(sse_test
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test/
)
