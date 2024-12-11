add_library(dcg src/dcg.c)

target_include_directories(dcg PUBLIC include)
target_link_libraries(dcg PUBLIC ext)

if (OPENMP_FOUND)
  target_link_libraries(dcg PUBLIC OpenMP::OpenMP_C)
endif()

add_executable(dcg_asp_test test/asp_test.c)

target_link_libraries(dcg_asp_test dcg)
target_link_libraries(dcg_asp_test munit)

add_test(
  NAME dcg_asp_test 
  COMMAND dcg_asp_test
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bgs/dcg/test
)
