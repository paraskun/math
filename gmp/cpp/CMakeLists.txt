project(gmp CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -Wextra
                    -std=c++23")

set(SRC src/gmp_als.cc)
set(HDR include/gmp_als.hpp)

add_library(gmp_cpp ${SRC} ${HDR})

target_include_directories(gmp_cpp PUBLIC ./include/)
