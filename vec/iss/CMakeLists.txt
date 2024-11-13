set(SRC 
  src/iss.c 
  src/iss_cds.c 
  src/iss_csj.c
)

set(HDR 
  include/iss.h 
  include/iss_cds.h 
  include/iss_csj.h
)

add_library(iss ${SRC} ${HDR})
target_link_libraries(iss PUBLIC vec)
target_link_libraries(iss PUBLIC mtx)
target_link_libraries(iss PUBLIC dss)
target_include_directories(iss PUBLIC include/)

add_executable(iss_csj_test test/iss_csj_test.cc)
target_link_libraries(iss_csj_test mtx iss GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(iss_csj_test
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vec/iss/test/
)
