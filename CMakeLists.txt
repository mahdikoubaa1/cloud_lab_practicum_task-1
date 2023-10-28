cmake_minimum_required(VERSION 3.14)
project(cloudlab
        VERSION 0.1
        DESCRIPTION "TUM Cloud Computing Lab"
        LANGUAGES CXX)

include(FetchContent)

# set compile flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} ${CMAKE_CXX_FLAGS_DEBUG} -fprofile-instr-generate -fcoverage-mapping")

add_compile_options(-Wall -Wextra -pedantic -Wformat=2 -g)
# detect dependencies
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)
find_package(fmt REQUIRED)
find_library(CLOUDLAB_ROCKSDB rocksdb REQUIRED)
if (CLOUDLAB_ROCKSDB)
    message(STATUS "Found rocksdb: " ${CLOUDLAB_ROCKSDB})
else ()
    message(FATAL_ERROR "rocksdb not found")
endif ()
find_library(CLOUDLAB_LIBEVENT event REQUIRED)
if (CLOUDLAB_LIBEVENT)
    message(STATUS "Found libevent: " ${CLOUDLAB_LIBEVENT})
else ()
    message(FATAL_ERROR "libevent not found")
endif ()
find_library(CLOUDLAB_LIBEVENT_PTHREADS event_pthreads REQUIRED)
if (CLOUDLAB_LIBEVENT_PTHREADS)
    message(STATUS "Found libevent_pthreads: " ${CLOUDLAB_LIBEVENT_PTHREADS})
else ()
    message(FATAL_ERROR "libevent_pthreads not found")
endif ()

# compile protobuf messages
protobuf_generate_cpp(PROTO_SRC PROTO_HDR ${PROJECT_SOURCE_DIR}/lib/message/cloud.proto)

# cloudlab library
add_library(cloudlab
        include/cloudlab/kvs.hh
        include/cloudlab/spmc.hh
        include/cloudlab/handler/api.hh
        include/cloudlab/handler/handler.hh
        include/cloudlab/handler/p2p.hh
        include/cloudlab/network/address.hh
        include/cloudlab/network/connection.hh
        include/cloudlab/network/routing.hh
        include/cloudlab/network/server.hh
        lib/kvs.cc
        lib/handler/api.cc
        lib/handler/p2p.cc
        lib/network/address.cc
        lib/network/connection.cc
        lib/network/server.cc
        ${PROTO_SRC}
        ${PROTO_HDR})
target_include_directories(cloudlab
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        PUBLIC ${PROTOBUF_INCLUDE_DIR}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(cloudlab
        PUBLIC ${PROTOBUF_LIBRARY}
        PRIVATE fmt::fmt
        PRIVATE Threads::Threads
        PRIVATE ${CLOUDLAB_ROCKSDB}
        PRIVATE ${CLOUDLAB_LIBEVENT}
        PRIVATE ${CLOUDLAB_LIBEVENT_PTHREADS})

# ctl executable
add_executable(ctl src/ctl.cc src/argh.hh)
target_link_libraries(ctl cloudlab fmt::fmt)

# kvs executable
add_executable(kvs src/kvs.cc src/argh.hh)
target_link_libraries(kvs cloudlab fmt::fmt)

# client executable
add_executable(client src/my_client.cc src/argh.hh)
target_link_libraries(client cloudlab fmt::fmt)

# verifier executable
add_executable(verifier src/verifier.cc src/argh.hh)
target_link_libraries(verifier cloudlab fmt::fmt)

# tests
enable_testing()

# tests for task1
add_executable(task1 test/unit_tests/task1.cc)
target_include_directories(task1 PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(task1 cloudlab GTest::gtest GTest::gtest_main fmt::fmt)