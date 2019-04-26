
project(wolcon)

file(GLOB SOURCES
        src/*.cpp
        src/*.h
        src/Platform/*.cpp
        src/Platform/*.h
        src/Wolcon/*.cpp
        src/Wolcon/*.h
        src/Wolcon/Events/*.h
        src/Wolcon/Events/*.cpp)

add_executable(Wolcon WIN32
        ${SOURCES})