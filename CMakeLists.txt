cmake_minimum_required(VERSION 3.10)
project(math C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -Wpedantic -std=c23")
set(C_STANDARD C23)

# add_subdirectory(vec)
add_subdirectory(gmp)
# add_subdirectory(ode) add_subdirectory(pde) add_subdirectory(cmd)
