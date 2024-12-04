set(SRC
  src/ipque.c
  src/ppque.c
  src/issll.c
  src/pssll.c
)

add_library(gds ${SRC})
target_include_directories(gds PUBLIC include/)
