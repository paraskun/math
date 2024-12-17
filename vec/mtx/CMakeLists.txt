set(SRC 
  src/mtx.c
  # src/mtx_cds.c
  # src/mtx_sky.c
  # src/mtx_csj.c
)

add_library(mtx ${SRC} ${HDR})

target_link_libraries(mtx PUBLIC vec)
target_link_libraries(mtx PUBLIC -lm)

target_include_directories(mtx PUBLIC include/)
