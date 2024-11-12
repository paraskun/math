set(SRC 
  src/dss_sky.c
  src/dss_csj.c
)

set(HDR 
  include/dss_sky.h
  include/dss_csj.h
)

add_library(dss ${SRC} ${HDR})
target_link_libraries(dss PUBLIC vec)
target_link_libraries(dss PUBLIC mtx)
target_include_directories(dss PUBLIC include/)

add_executable(dss_csj_test test/dss_csj_test.cc)
target_link_libraries(dss_csj_test mtx dss GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(dss_csj_test)
