set(SRC 
  src/mtx_cds.c
  src/mtx_sky.c
  src/mtx_pie.c
  src/mtx_csj.c
  src/mtx_all.c
)

set(HDR 
  include/mtx_cds.h
  include/mtx_sky.h
  include/mtx_pie.h
  include/mtx_csj.h
  include/mtx_all.h
)

add_library(mtx ${SRC} ${HDR})

target_link_libraries(mtx PUBLIC vec -lm)
target_include_directories(mtx PUBLIC include/)
