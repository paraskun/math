set(SRC src/iss_cds.c)

set(HDR include/iss_cds.h)

add_library(iss ${SRC} ${HDR})

target_link_libraries(iss PRIVATE vec)
target_link_libraries(iss PRIVATE mtx)

target_include_directories(iss PUBLIC include/)
