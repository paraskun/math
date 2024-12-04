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
