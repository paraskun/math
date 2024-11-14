set(SRC
  src/dcg_als.c
)

set(HDR
  include/dcg_als.h
)

add_library(dcg ${SRC} ${HDR})
target_include_directories(dcg PUBLIC include)

if (OPENMP_FOUND)
  target_link_libraries(dcg PUBLIC OpenMP::OpenMP_C)
endif()

add_executable(dcg_als_test test/dcg_als_test.cc)
target_link_libraries(dcg_als_test dcg GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(dcg_als_test)
