set(SRC
  src/ipque.c
  src/ppque.c
  src/ihset.c
  src/phset.c
  src/isll.c
  src/psll.c
)

add_library(dsa ${SRC})

target_include_directories(dsa PUBLIC include/)

add_executable(pque_test test/pque_test.c)

target_link_libraries(pque_test dsa)
target_link_libraries(pque_test munit)

add_test(NAME pque_test COMMAND pque_test)
