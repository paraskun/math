file(GLOB SRC src/*.c)

add_library(non ${SRC})

target_link_libraries(non vec)
target_include_directories(calc PUBLIC include/)

add_executable(test_non test/test_non.c)

target_link_libraries(test_non non munit)

add_test(
  NAME test_non
  COMMAND test_non
)
