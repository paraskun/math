set(SRC 
  src/mtx.c
  src/mtx_cds.c
  src/mtx_sky.c
  src/mtx_csj.c
)

add_library(mtx ${SRC} ${HDR})

target_link_libraries(mtx PUBLIC vec)
target_link_libraries(mtx PUBLIC -lm)

target_include_directories(mtx PUBLIC include/)

# add_executable(CompressedSparseJointTest 
#   test/CompressedSparseJointTest.cc
# )
# 
# target_link_libraries(CompressedSparseJointTest mtx GTest::gtest_main)
# 
# include(GoogleTest)
# gtest_discover_tests(CompressedSparseJointTest
#   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vec/mtx/test/
# )
