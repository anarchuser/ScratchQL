cmake_minimum_required(VERSION 3.12.4)
project(Database)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-lglog")

if("${OS_INFO}" MATCHES "buntu")
    message("Running on Ubuntu - manually linking SDL2!")
    set(UBUNTU 1)
endif()

if(UBUNTU)
    set(GLOG /usr/lib/x86_64-linux-gnu/libgoogle-glog.so)

    target_link_libraries(eve ${GLOG})
    target_link_libraries(tests ${GLOG})
endif()

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*/*.cpp)
file(GLOB_RECURSE TEST_SRCS ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(Database src/main.cpp ${SRCS})
add_executable(Test ${TEST_SRCS} ${SRCS})

find_package(Catch2 REQUIRED)
target_link_libraries(Test Catch2::Catch2)
include(CTest)
include(Catch)
catch_discover_tests(Test)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Database Threads::Threads)
target_link_libraries(Test Threads::Threads)