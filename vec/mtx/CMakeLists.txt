set(SRC 
  src/mtx.c
  src/mtx_cds.c
  src/mtx_sky.c
  src/mtx_pie.c
  src/mtx_csj.c
)

add_library(mtx ${SRC} ${HDR})
target_link_libraries(mtx PUBLIC vec -lm)
target_include_directories(mtx PUBLIC include/)

add_executable(mtx_csj_test test/mtx_csj_test.cc)
target_link_libraries(mtx_csj_test mtx GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mtx_csj_test
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vec/mtx/test/
)
