add_library(dcg src/dcg.c)

target_include_directories(dcg PUBLIC include)
target_link_libraries(dcg PUBLIC gds)

if (OPENMP_FOUND)
  target_link_libraries(dcg PUBLIC OpenMP::OpenMP_C)
endif()

add_executable(dcg_test test/dcg/dcg_test.c)

target_link_libraries(dcg_test dcg)
target_link_libraries(dcg_test munit)

add_test(NAME dcg_test COMMAND dcg_test)
