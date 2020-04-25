cmake_minimum_required(VERSION 3.12.4)
project(Database)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_REQUIRED_LIBRARIES "-lglog -lcapnp -lcapnp-rpc -lkj -lkj-async")
set(CMAKE_CXX_FLAGS "-fconcepts")

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*/*.c*)
add_executable(Database src/main.cpp ${SRCS})

add_compile_definitions(Test ERRORS_AS_WARNINGS)

set(THREADS_PREFER_PTHREAD_FLAG ON)
target_link_libraries(Database "/usr/include/threads.h")
#find_package(Threads REQUIRED)
#target_link_libraries(Database Threads::Threads)

target_link_libraries(Database glog)

find_package(CapnProto REQUIRED)
target_link_libraries(Database CapnProto::capnp)
target_link_libraries(Database CapnProto::capnp-rpc)
target_link_libraries(Database kj)
target_link_libraries(Database kj-async)

find_package(Catch2)
IF (Catch2_FOUND)
    file(GLOB_RECURSE TEST_SRCS ${PROJECT_SOURCE_DIR}/test/*.c*)
    add_executable(Test ${TEST_SRCS} ${SRCS})

    target_link_libraries(Test Catch2::Catch2)
    include(CTest)
    include(Catch)
    catch_discover_tests(Test)

    #target_link_libraries(Test Threads::Threads)

    target_link_libraries(Test glog)

    target_link_libraries(Test "/usr/include/threads.h")

    target_link_libraries(Test CapnProto::capnp)
    target_link_libraries(Test CapnProto::capnp-rpc)
    target_link_libraries(Test kj)
    target_link_libraries(Test kj-async)
ENDIF()

