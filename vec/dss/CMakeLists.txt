set(SRC 
  src/dss_sky.c
)

set(HDR 
  include/dss_sky.h
)

add_library(dss ${SRC} ${HDR})

target_link_libraries(dss PUBLIC vec)
target_link_libraries(dss PUBLIC mtx)

target_include_directories(dss PUBLIC include/)
