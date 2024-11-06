set(SRC 
  src/dfs.c
)

set(HDR 
  include/dfs.h
)

add_library(dfs ${SRC} ${HDR})

target_link_libraries(dfs PUBLIC vec)
target_link_libraries(dfs PUBLIC mtx)

target_include_directories(dfs PUBLIC include/)
