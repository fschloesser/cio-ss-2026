cmake_minimum_required(VERSION 4.0)
project(shapes CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(draw main.cpp circle.cpp square.cpp triangle.cpp)
