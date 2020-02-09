cmake_minimum_required(VERSION 3.14.1)

project(vice LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

option(USE_OEHEADLESS "Compile OpenEmu headless library" ON)
option(USE_ALT_CPU "Use alternate CPU" ON)

list(APPEND ARCH_INCLUDES
        ${CMAKE_SOURCE_DIR}/src/arch
        )

if (USE_ALT_CPU)
    add_definitions(-DUSE_ALT_CPU)
endif ()

message("TYPE: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("adding lto")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto=full -march=skylake -mavx2 -ffast-math")
endif()

if (USE_OEHEADLESS)
    list(APPEND ARCH_INCLUDES
            ${CMAKE_SOURCE_DIR}/src/arch/oeheadless
            )
    add_definitions(-DUSE_OEHEADLESS)
    set(ARCH_LIBS oeheadless)
else ()
    list(APPEND ARCH_INCLUDES
            ${CMAKE_SOURCE_DIR}/src/arch/sdl
            ${CMAKE_SOURCE_DIR}/src/arch/shared
            /usr/local/include
            )
    add_definitions(-DUSE_SDLUI2 -DUSE_SDL_AUDIO -DUSE_COREAUDIO)
    set(ARCH_LIBS SDL2 sdl archdep)
    link_directories(/usr/local/lib)
endif ()

add_subdirectory(src)