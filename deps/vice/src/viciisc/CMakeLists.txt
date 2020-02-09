set(VICIISC_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/raster
    ${CMAKE_SOURCE_DIR}/src/joyport
    ${CMAKE_SOURCE_DIR}/src/c64
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    ${CMAKE_SOURCE_DIR}/src/c64/cart
    )

set(VICIISC_SOURCES
    vicii-chip-model.c
    vicii-chip-model.h
    vicii-cmdline-options.c
    vicii-cmdline-options.h
    vicii-color.c
    vicii-color.h
    vicii-cycle.c
    vicii-cycle.h
    vicii-draw.c
    vicii-draw.h
    vicii-draw-cycle.c
    vicii-draw-cycle.h
    vicii-fetch.c
    vicii-fetch.h
    vicii-irq.c
    vicii-irq.h
    vicii-lightpen.c
    vicii-lightpen.h
    vicii-mem.c
    vicii-mem.h
    vicii-phi1.c
    vicii-phi1.h
    vicii-resources.c
    vicii-resources.h
    vicii-snapshot.c
    vicii-snapshot.h
    vicii-timing.c
    vicii-timing.h
    vicii.c
    viciitypes.h
    )

add_library(viciisc STATIC ${VICIISC_SOURCES})
target_include_directories(viciisc PRIVATE ${VICIISC_INCLUDES})
