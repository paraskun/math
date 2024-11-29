set(SRC
  src/fem.c
  src/vtx.c
  src/fce.c
  src/hex.c
  src/ull.c
)

add_library(sse ${SRC})

target_link_libraries(sse PUBLIC mtx)
target_link_libraries(sse PUBLIC iss)

target_include_directories(sse PUBLIC include/)

add_executable(SolidStateEquationSingleElementTest 
  test/SolidStateEquationSingleElementTest.cc)

add_executable(SolidStateEquationInnerNodeTest 
  test/SolidStateEquationInnerNodeTest.cc)

add_executable(SolidStateEquationFractureTest 
  test/SolidStateEquationFractureTest.cc)

target_link_libraries(SolidStateEquationSingleElementTest sse)
target_link_libraries(SolidStateEquationSingleElementTest GTest::gtest_main)

target_link_libraries(SolidStateEquationInnerNodeTest sse)
target_link_libraries(SolidStateEquationInnerNodeTest GTest::gtest_main)

target_link_libraries(SolidStateEquationFractureTest sse)
target_link_libraries(SolidStateEquationFractureTest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(SolidStateEquationSingleElementTest
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test/
)

include(GoogleTest)
gtest_discover_tests(SolidStateEquationInnerNodeTest
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test/
)

include(GoogleTest)
gtest_discover_tests(SolidStateEquationFractureTest
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/pde/fem/sse/test/
)
