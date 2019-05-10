cmake_minimum_required(VERSION 3.12)
project(legv8Sim)

set(CMAKE_CXX_STANDARD 17)

add_executable(legv8Sim main.cpp legv8Line.h legv8Line.cpp legv8Sim.h legv8Sim.cpp)