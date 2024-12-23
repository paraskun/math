# file(GLOB SRC src/*.c)

set(SRC
  src/imtx.c
  src/jmtx.c
)

add_library(mtx ${SRC})

target_link_libraries(mtx PUBLIC vec)
target_link_libraries(mtx PUBLIC -lm)

target_include_directories(mtx PUBLIC include/)
