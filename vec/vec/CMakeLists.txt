set(SRC
  src/vec.c
)

add_library(vec ${SRC} ${HDR})
target_include_directories(vec PUBLIC include/)

if (OPENMP_FOUND)
  target_link_libraries(vec PUBLIC OpenMP::OpenMP_C)
endif()

# add_executable(VectorTest test/VectorTest.cc)
# target_link_libraries(VectorTest vec GTest::gtest_main)
# 
# include(GoogleTest)
# gtest_discover_tests(VectorTest)
