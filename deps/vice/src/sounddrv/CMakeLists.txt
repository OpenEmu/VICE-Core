set(SOUNDDRV_INCLUDES
        ${ARCH_INCLUDES}
        ${CMAKE_SOURCE_DIR}/src
        )

set(SOUNDDRV_SOURCES
        soundcoreaudio.c
        soundsdl.c
        soundaiff.c
        sounddummy.c
        sounddump.c
        soundfs.c
        soundiff.c
        soundmovie.c
        soundvoc.c
        soundwav.c
        )

if (USE_OEHEADLESS)
    list(APPEND SOUNDDRV_SOURCES
            soundheadless.m
            )
endif ()

add_library(sounddrv STATIC ${SOUNDDRV_SOURCES})
target_include_directories(sounddrv PRIVATE ${SOUNDDRV_INCLUDES})

