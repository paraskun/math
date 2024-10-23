set(SRC src/mtx_cds.c)

set(HDR include/mtx_cds.h)

add_library(mtx ${SRC} ${HDR})

target_link_libraries(mtx PUBLIC vec -lm)
target_include_directories(mtx PUBLIC include/)
