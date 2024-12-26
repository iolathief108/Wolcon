
project(wolcon)

file(GLOB SOURCES
        src/*.cpp
        src/*.h)

set(DEBUG FALSE)

if (${DEBUG})
    add_executable(Wolcon ${SOURCES})
    add_compile_definitions(CONSOLE)
else()
    add_executable(Wolcon WIN32 ${SOURCES})
endif()