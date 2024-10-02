set(SRC
  src/vec.c
  src/dmtx.c
  src/smtx.c
  src/dsle.c
  src/ssle.c
)

set(HDRS
  include/vec.h
  include/mtx.h
  include/sle.h
  include/dmtx.h
  include/smtx.h
  include/dsle.h
  include/ssle.h
)

set(MATH_REAL double)   # default
set(MATH_PREAL double)  # default

add_library(math ${SRC} ${HDRS})
target_include_directories(math PUBLIC include/)

target_compile_definitions(math PUBLIC real=${MATH_REAL})
target_compile_definitions(math PUBLIC preal=${MATH_PREAL})

target_compile_options(math PUBLIC ${COMPILE_OPTIONS})

find_package(OpenMP)

if (OpenMP_FOUND)
  target_link_libraries(math PRIVATE OpenMP::OpenMP_C)
endif()

target_link_libraries(math PRIVATE m)

