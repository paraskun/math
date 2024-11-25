set(SRC
  src/fem.c
)

set(HDR
  include/fem.h
  include/obj.h
)

add_library(sse ${SRC} ${HDR})
target_link_libraries(sse mtx)
target_include_directories(sse PUBLIC include/)
