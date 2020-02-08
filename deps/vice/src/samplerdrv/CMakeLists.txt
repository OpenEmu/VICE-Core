set(SAMPLERDRV_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    )

set(SAMPLERDRV_SOURCES
    file_drv.c
    file_drv.h
    portaudio_drv.c
    portaudio_drv.h
    sampler.c
    sampler.h
    )

add_library(samplerdrv STATIC ${SAMPLERDRV_SOURCES})
target_include_directories(samplerdrv PRIVATE ${SAMPLERDRV_INCLUDES})

