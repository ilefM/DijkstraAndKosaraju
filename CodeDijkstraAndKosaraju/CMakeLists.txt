cmake_minimum_required(VERSION 3.5)
project(TP2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Graphe.cpp
    Graphe.h
    ReseauInterurbain.cpp
    ReseauInterurbain.h
    Principal.cpp)

add_executable(TP2 ${SOURCE_FILES})