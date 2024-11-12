set(SRC
  src/vec.c
)

set(HDR
  include/vec.h
)

add_library(vec ${SRC} ${HDR})

target_include_directories(vec PUBLIC include/)

if (OPENMP_FOUND)
  target_link_libraries(vec PUBLIC OpenMP::OpenMP_C)
endif()

add_executable(vec_test test/vec_test.cc)
target_link_libraries(vec_test vec GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(vec_test)

add_subdirectory(dfs)
add_subdirectory(dss)
add_subdirectory(iss)
add_subdirectory(mtx)
