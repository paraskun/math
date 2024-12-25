file(GLOB SRC src/*.c)

add_library(calc ${SRC})

target_link_libraries(calc vec)
target_include_directories(calc PUBLIC include/)

add_executable(test_calc test/test_calc.c)

target_link_libraries(test_calc calc munit)

add_test(
  NAME test_calc
  COMMAND test_calc
)
