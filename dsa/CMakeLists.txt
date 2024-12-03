set(SRC
  src/heap.c
)

add_library(dsa ${SRC})

target_include_directories(dsa PUBLIC include/)
