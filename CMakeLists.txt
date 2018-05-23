project(mcache C)
cmake_minimum_required(VERSION 2.8)

add_library(ngx_queue STATIC ngx_queue.c)
add_library(ngx_rbtree STATIC ngx_rbtree.c)
add_library(mcache STATIC mcache.c)

add_executable(test_main test.c mcache.c)
target_link_libraries(test_main ngx_queue ngx_rbtree mcache pthread)