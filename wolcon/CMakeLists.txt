
project(wolcon)

file(GLOB SOURCES
        src/*.cpp
        src/*.h)

add_executable(Wolcon WIN32 ${SOURCES})