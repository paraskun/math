set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

set(SRC
  src/hw.c
)

set(HDRS
  include/hw.h
)

add_library(prll ${SRC} ${HDRS})

target_include_directories(prll PUBLIC include)

find_package(MPI REQUIRED)
target_link_libraries(prll ${MPI_C_LIBRARIES})
target_link_libraries(prll math)
