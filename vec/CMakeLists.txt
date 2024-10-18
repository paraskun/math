set(SRC
    src/mtx_dns.c
    src/mtx_dns.h

    src/mtx_pfl.c
    src/mtx_pfl.h

    src/mtx_pie.c
    src/mtx_pie.h

    src/sle_dns.c
    src/sle_dns.h

    src/sle_pfl.c
    src/sle_pfl.h

    src/vec.c)

add_library(vec ${SRC})

set(REAL "double")
set(SYNT "double")
set(RFMT "\"%lf\"")

target_compile_definitions(vec PUBLIC real=${REAL})
target_compile_definitions(vec PUBLIC synt=${SYNT})
target_compile_definitions(vec PUBLIC FMT=${RFMT})

target_include_directories(vec PUBLIC include/)
target_include_directories(vec PUBLIC src/)

target_link_libraries(vec PUBLIC -lm)
