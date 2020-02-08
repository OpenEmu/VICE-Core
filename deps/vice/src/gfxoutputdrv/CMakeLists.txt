set(GFXOUTPUTDRV_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/arch/shared
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/shared
    )

set(GFXOUTPUTDRV_SOURCES
    ffmpegdrv.c
    ffmpegdrv.h
    ffmpeglib.c
    ffmpeglib.h
    gifdrv.c
    gifdrv.h
    jpegdrv.c
    jpegdrv.h
    pngdrv.c
    pngdrv.h
    quicktimedrv.h
    quicktimedrv.c
    bmpdrv.c
    bmpdrv.h
    doodledrv.c
    gfxoutput.c
    godotdrv.c
    godotdrv.h
    iffdrv.c
    iffdrv.h
    koaladrv.c
    nativedrv.c
    nativedrv.h
    pcxdrv.c
    pcxdrv.h
    ppmdrv.c
    ppmdrv.h
    )

add_library(gfxoutputdrv STATIC ${GFXOUTPUTDRV_SOURCES})
target_include_directories(gfxoutputdrv PRIVATE ${GFXOUTPUTDRV_INCLUDES})

