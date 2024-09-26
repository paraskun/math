set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

set(SRC
  src/vec.c
  src/dmtx.c
  src/smtx.c
  src/ssle.c
  src/dsle.c
)

set(HDRS
  include/vec.h
  include/mtx.h
  include/sle.h
)

add_library(math ${SRC} ${HDRS})

target_compile_definitions(math PUBLIC real=double)
target_compile_definitions(math PUBLIC preal=double)
target_compile_definitions(math PUBLIC OMP_THREADS_NUM=2)
target_compile_definitions(math PUBLIC SMTX)

target_include_directories(math PUBLIC include/)
target_link_libraries(math PRIVATE m)

find_package(OpenMP)

if (OpenMP_FOUND)
  target_link_libraries(math PRIVATE OpenMP::OpenMP_C)
endif()

