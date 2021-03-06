cmake_minimum_required(VERSION 3.12.4)
project(Database)


set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)

set(CAPNP_LIBS "-lcapnp -lcapnp-rpc -lkj -lkj-async")
set(CMAKE_REQUIRED_LIBRARIES "${CAPNP_LIBS} -lglog -pthread")
set(CMAKE_CXX_FLAGS "-fconcepts")

set(GEN_PROTO_FILES src/Server/generated/ServerDBMS.capnp.*)
add_custom_command(OUTPUT ${GEN_PROTO_FILES}
        COMMAND bash -c "./gen_proto"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/src/Server/"
        MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/src/Server/ServerDBMS.capnp
        VERBATIM)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*/*.c*)
add_executable(Database src/main.cpp ${GE_PROTO_FILES} ${SRCS})

install()

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
target_link_libraries(Database Threads::Threads)

target_link_libraries(Database glog)

find_package(CapnProto REQUIRED)
target_link_libraries(Database CapnProto::capnp)
target_link_libraries(Database CapnProto::capnp-rpc)
target_link_libraries(Database kj)
target_link_libraries(Database kj-async)

find_package(Catch2 CONFIG)
IF (Catch2_FOUND)
    file(GLOB_RECURSE TEST_SRCS ${PROJECT_SOURCE_DIR}/test/*.c*)
    add_executable(Test ${GEN_PROTO_FILES} ${TEST_SRCS}  ${SRCS})

    add_compile_definitions(Test PRIVATE ERRORS_AS_WARNINGS)

    target_link_libraries(Test PRIVATE Catch2::Catch2)
    include(CTest)
    include(Catch)
    catch_discover_tests(Test)

    target_link_libraries(Test PRIVATE glog)

    target_link_libraries(Test PRIVATE Threads::Threads)

    target_link_libraries(Test PRIVATE CapnProto::capnp)
    target_link_libraries(Test PRIVATE CapnProto::capnp-rpc)
    target_link_libraries(Test PRIVATE kj)
    target_link_libraries(Test PRIVATE kj-async)
ENDIF()
