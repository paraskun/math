file(GLOB SRC src/*.c)

add_library(bas ${SRC})

target_link_libraries(bas vec)
target_include_directories(bas PUBLIC include/)

add_executable(test_mas test/test_mas.c)

target_link_libraries(test_mas bas munit)

add_test(
  NAME test_mas
  COMMAND test_mas
)
