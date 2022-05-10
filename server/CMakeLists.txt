cmake_minimum_required(VERSION 3.0.0)

project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

set(SRC_LIST main.cpp sources/server_functions.cpp sources/server.cpp )
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

add_executable(${PROJECT_NAME} ${SRC_LIST})
