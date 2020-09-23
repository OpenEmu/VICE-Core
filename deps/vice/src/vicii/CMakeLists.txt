set(VICII_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/raster
    ${CMAKE_SOURCE_DIR}/src/joyport
    ${CMAKE_SOURCE_DIR}/src/c64
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    ${CMAKE_SOURCE_DIR}/src/c64/cart
    ${CMAKE_SOURCE_DIR}/src/c64dtv
    )

set(VICII_SOURCES
    vicii-badline.c
    vicii-badline.h
    vicii-clock-stretch.c
    vicii-cmdline-options.c
    vicii-cmdline-options.h
    vicii-color.c
    vicii-color.h
    vicii-draw.c
    vicii-draw.h
    vicii-fetch.c
    vicii-fetch.h
    vicii-irq.c
    vicii-irq.h
    vicii-mem.c
    vicii-mem.h
    vicii-phi1.c
    vicii-phi1.h
    vicii-resources.c
    vicii-resources.h
    vicii-snapshot.c
    vicii-snapshot.h
    vicii-sprites.c
    vicii-sprites.h
    vicii-stubs.c
    vicii-timing.c
    vicii-timing.h
    vicii.c
    viciidtv-color.c
    viciidtv-draw.c
    viciidtv-snapshot.c
    viciitypes.h
    )

add_library(vicii STATIC ${VICII_SOURCES})
target_include_directories(vicii PRIVATE ${VICII_INCLUDES})
