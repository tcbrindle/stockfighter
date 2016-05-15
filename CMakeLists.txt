
cmake_minimum_required(VERSION 3.0)

project(stockfighter CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(OpenSSL REQUIRED)
find_package(cppnetlib 0.12 REQUIRED)
find_package(cppformat REQUIRED)

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
