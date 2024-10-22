add_library(vec vec.c vec.h)

target_include_directories(vec PUBLIC .)

add_subdirectory(dfs)
add_subdirectory(dss)
add_subdirectory(iss)
add_subdirectory(mtx)
