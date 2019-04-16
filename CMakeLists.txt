cmake_minimum_required(VERSION 3.12)
project(Wolcon)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
        src/*.cpp
        src/*.h)

add_executable(Wolcon WIN32
        ${SOURCES})