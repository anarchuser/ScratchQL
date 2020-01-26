cmake_minimum_required(VERSION 3.16)
project(Database)

set(CMAKE_CXX_STANDARD 20)

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