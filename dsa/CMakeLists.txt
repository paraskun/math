set(SRC
  src/pque/ipque.c
  src/pque/upque.c
  src/pque/dpque.c
  src/pque/ppque.c

  src/hset/ihset.c

  src/sll/isll.c
  src/sll/psll.c
)

add_library(dsa ${SRC})

target_include_directories(dsa PUBLIC include/)

add_executable(pque_test test/pque_test.c)

target_link_libraries(pque_test dsa)
target_link_libraries(pque_test munit)

add_test(NAME pque_test COMMAND pque_test)
