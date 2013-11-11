project(memcbench)
cmake_minimum_required(VERSION 2.6)

set(SOURCE src/memcbench.cc src/opt.cc)

find_library(LIBMEMCACHED_LIBRARY NAMES memcached PATHS /usr/local/lib)
find_path(LIBMEMCACHED_INCLUDE_DIR memcached.h /usr/local/include/libmemcached)
include_directories(${LIBMEMCACHED_INCLUDE_DIR})

add_executable(memcbench ${SOURCE})
target_link_libraries(memcbench ${LIBMEMCACHED_LIBRARY} pthread)

add_executable(memcinsert src/memcinsert.cc)
target_link_libraries(memcinsert ${LIBMEMCACHED_LIBRARY} pthread)

install(TARGETS memcbench DESTINATION bin)
install(TARGETS memcinsert DESTINATION bin)
