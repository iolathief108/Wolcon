
project(wolcon)

file(GLOB SOURCES
        src/*.cpp
        src/*.h
        src/Events/*.cpp
        src/Events/*.h)

add_executable(Wolcon WIN32 ${SOURCES})