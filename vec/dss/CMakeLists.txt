set(SRC 
  src/dss_sky.c
  src/dss_csj.c
)

set(HDR 
  include/dss_sky.h
  include/dss_csj.h
)

add_library(dss ${SRC} ${HDR})
target_link_libraries(dss PUBLIC vec)
target_link_libraries(dss PUBLIC mtx)
target_include_directories(dss PUBLIC include/)
