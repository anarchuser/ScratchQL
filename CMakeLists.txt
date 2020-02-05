cmake_minimum_required(VERSION 3.12.4)
project(Database)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_REQUIRED_LIBRARIES "-lglog")
if(NOT CXX_FILESYSTEM_STDCPPFS_NEEDED)
   set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} -lc++fs)
endif()

execute_process(COMMAND cat /etc/os-release
        OUTPUT_VARIABLE OS_INFO)
if("${OS_INFO}" MATCHES "buntu")
    message("Running on Ubuntu (important for Travis CI")
    set(UBUNTU 1)
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

target_link_libraries(Database glog)
target_link_libraries(Test glog)
