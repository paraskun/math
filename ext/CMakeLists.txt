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

add_library(ext ${SRC})

target_include_directories(ext PUBLIC include/)
