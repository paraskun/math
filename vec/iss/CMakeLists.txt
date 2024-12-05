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

add_executable(iss_csj_test test/csj/csj_test.c)

target_link_libraries(iss_csj_test PUBLIC munit)
target_link_libraries(iss_csj_test PUBLIC iss)

add_test(
  NAME iss_csj_test 
  COMMAND iss_csj_test
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vec/iss/test/csj
)
