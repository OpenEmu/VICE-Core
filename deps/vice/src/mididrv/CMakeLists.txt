set(MIDIDRV_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/arch/shared
    ${CMAKE_SOURCE_DIR}/src
    )

set(MIDIDRV_SOURCES
    midi-osx-drv.c
    )

add_library(mididrv STATIC ${MIDIDRV_SOURCES})
target_include_directories(mididrv PRIVATE ${MIDIDRV_INCLUDES})

