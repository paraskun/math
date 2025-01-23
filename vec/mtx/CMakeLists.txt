file(GLOB SRC src/*.c)

add_library(mtx ${SRC})

target_link_libraries(mtx PUBLIC vec non -lm)
target_include_directories(mtx PUBLIC include/)
