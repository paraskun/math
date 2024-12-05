set(SRC
  src/vec.c
)

add_library(vec ${SRC} ${HDR})
target_include_directories(vec PUBLIC include/)

if (OPENMP_FOUND)
  target_link_libraries(vec PUBLIC OpenMP::OpenMP_C)
endif()
