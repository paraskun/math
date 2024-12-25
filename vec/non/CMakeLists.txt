file(GLOB SRC src/*.c)

add_library(non ${SRC})

target_link_libraries(non PUBLIC vec mtx calc dss stdx)
target_include_directories(non PUBLIC include/)

add_executable(test_new test/test_new.c)

target_link_libraries(test_new non munit)

add_test(
  NAME test_new
  COMMAND test_new
)

add_executable(test_new_vis1 test/test_new_vis1.c)
add_executable(test_new_vis2 test/test_new_vis2.c)
add_executable(test_new_vis3 test/test_new_vis3.c)
add_executable(test_new_vis4 test/test_new_vis4.c)
add_executable(test_new_vis5 test/test_new_vis5.c)
add_executable(test_new_vis6 test/test_new_vis6.c)
add_executable(test_new_vis7 test/test_new_vis7.c)
add_executable(test_new_vis8 test/test_new_vis8.c)
add_executable(test_new_vis9 test/test_new_vis9.c)

target_link_libraries(test_new_vis1 non)
target_link_libraries(test_new_vis2 non)
target_link_libraries(test_new_vis3 non)
target_link_libraries(test_new_vis4 non)
target_link_libraries(test_new_vis5 non)
target_link_libraries(test_new_vis6 non)
target_link_libraries(test_new_vis7 non)
target_link_libraries(test_new_vis8 non)
target_link_libraries(test_new_vis9 non)
