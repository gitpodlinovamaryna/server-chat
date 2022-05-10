cmake_minimum_required(VERSION 3.0.0)

project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SRC_LIST main.cpp sources/client_functions.cpp sources/client.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})