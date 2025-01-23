file(GLOB SRC src/*.c)

add_library(non ${SRC})

target_link_libraries(non vec dss stdx)
target_include_directories(non PUBLIC include/)
