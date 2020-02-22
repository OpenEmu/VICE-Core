set(IMAGECONTENTS_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/vdrive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(IMAGECONTENTS_SOURCES
    diskcontents-block.c
    diskcontents-block.h
    diskcontents-iec.c
    diskcontents-iec.h
    diskcontents.c
    diskcontents.h
    imagecontents.c
    tapecontents.c
    tapecontents.h
    )

add_library(imagecontents STATIC ${IMAGECONTENTS_SOURCES})
target_include_directories(imagecontents PRIVATE ${IMAGECONTENTS_INCLUDES})

