set(SRC
  src/pque/ipque.c
  src/pque/upque.c
  src/pque/dpque.c
  src/pque/ppque.c

  src/hset/ihset.c
  src/hset/uhset.c

  src/sll/isll.c
  src/sll/psll.c
)

add_library(dsa ${SRC})

target_include_directories(dsa PUBLIC include/)

add_executable(pque_test test/pque_test.c)
add_executable(hset_test test/hset_test.c)
add_executable(sll_test test/sll_test.c)

target_link_libraries(pque_test dsa munit)
target_link_libraries(hset_test dsa munit)
target_link_libraries(sll_test dsa munit)

add_test(NAME pque_test COMMAND pque_test)
add_test(NAME hset_test COMMAND hset_test)
add_test(NAME sll_test COMMAND sll_test)
