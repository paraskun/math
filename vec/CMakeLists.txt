set(SRC
    src/dmtx.c
    src/dmtx.h
    src/smtx.c
    src/smtx.h
    src/pmtx.c
    src/pmtx.h
    src/dsle.c
    src/dsle.h
    src/ssle.c
    src/ssle.h
    src/vec.c)

add_library(vec ${SRC})

set(REAL "double")
set(SYNT "double")
set(RFMT "\"%lf\"")

target_compile_definitions(vec PUBLIC real=${REAL})
target_compile_definitions(vec PUBLIC synt=${SYNT})
target_compile_definitions(vec PUBLIC FMT=${RFMT})

target_include_directories(vec PUBLIC include/)
target_include_directories(vec PRIVATE src/)
