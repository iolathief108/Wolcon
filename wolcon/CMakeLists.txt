
project(wolcon)

file(GLOB SOURCES
        src/*.cpp
        src/*.h
        src/Events/*.cpp
        src/Events/*.h)

add_definitions(-DWL_PLATFORM_WINDOWS)

add_executable(Wolcon WIN32 ${SOURCES})

include_directories(src)