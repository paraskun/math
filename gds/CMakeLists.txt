set(SRC
  src/ipque.c
  src/ppque.c
  src/issll.c
  src/pssll.c
  src/ihset.c
  src/phset.c
)

add_library(gds ${SRC})

target_include_directories(gds PUBLIC include/)

add_executable(pque_test test/pque/pque_test.c)

target_link_libraries(pque_test gds)
target_link_libraries(pque_test munit)

add_test(NAME pque_test COMMAND pque_test)
