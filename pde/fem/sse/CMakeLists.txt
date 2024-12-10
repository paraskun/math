set(SRC
  src/fem.c
  src/vtx.c
  src/fce.c
  src/hex.c
  src/ull.c
)

add_library(sse ${SRC})

target_link_libraries(sse PUBLIC mtx)
target_link_libraries(sse PUBLIC iss)
target_link_libraries(sse PUBLIC ext)

target_include_directories(sse PUBLIC include/)
