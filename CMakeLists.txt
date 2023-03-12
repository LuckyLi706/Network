cmake_minimum_required(VERSION 3.14.3)
project(Network)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
add_executable(Network tcp_linux_centos_server.cpp)