set(SRC src/iss_cds.c src/iss_csr.c)
set(HDR include/iss_cmn.h include/iss_cds.h include/iss_csr.h)

add_library(iss ${SRC} ${HDR})

target_link_libraries(iss PUBLIC vec)
target_link_libraries(iss PUBLIC mtx)

target_include_directories(iss PUBLIC include/)
