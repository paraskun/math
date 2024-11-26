set(SRC
  src/fem.c
  src/vtx.c
  src/fce.c
  src/hex.c
)

add_library(sse ${SRC})
target_link_libraries(sse mtx iss)
target_include_directories(sse PUBLIC include/)
