set(SRC 
  src/iss.c 
  src/iss_cds.c 
  src/iss_csj.c
)

add_library(iss ${SRC} ${HDR})
target_link_libraries(iss PUBLIC vec)
target_link_libraries(iss PUBLIC mtx)
target_link_libraries(iss PUBLIC dss)
target_include_directories(iss PUBLIC include/)

# add_executable(CompressedSparseJointHilbertTest 
#   test/CompressedSparseJointHilbertTest.cc
# )
# 
# add_executable(CompressedSparseJointDominantTest 
#   test/CompressedSparseJointDominantTest.cc
# )
# 
# target_link_libraries(CompressedSparseJointHilbertTest mtx iss GTest::gtest_main)
# target_link_libraries(CompressedSparseJointDominantTest mtx iss GTest::gtest_main)
# 
# include(GoogleTest)
# gtest_discover_tests(CompressedSparseJointHilbertTest
#   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vec/iss/test/)
# gtest_discover_tests(CompressedSparseJointDominantTest
#   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vec/iss/test/)
