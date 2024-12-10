set(SRC
  src/const.h

  src/fem.c
  src/vtx.c
  src/fce.c
  src/hex.c
)

add_library(sse ${SRC})

target_link_libraries(sse PUBLIC mtx)
target_link_libraries(sse PUBLIC iss)
target_link_libraries(sse PUBLIC ext)

target_include_directories(sse PUBLIC include/)
