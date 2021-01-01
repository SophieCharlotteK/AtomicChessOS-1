cmake_minimum_required(VERSION 3.17)
project(controller)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DOSXDBG)
add_executable(controller controller.cpp)