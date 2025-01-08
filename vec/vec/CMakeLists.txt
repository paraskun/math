set(SRC src/vec.c)

add_library(vec ${SRC} ${HDR})

target_link_libraries(vec PUBLIC -lm)
target_include_directories(vec PUBLIC include/)
