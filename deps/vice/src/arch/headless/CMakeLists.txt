set(HEADLESS_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/crtc
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/joyport
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    ${CMAKE_SOURCE_DIR}/src/raster
    ${CMAKE_SOURCE_DIR}/src/rs232drv
    ${CMAKE_SOURCE_DIR}/src/sid
    ${CMAKE_SOURCE_DIR}/src/vdc
    ${CMAKE_SOURCE_DIR}/src/c64
    ${CMAKE_SOURCE_DIR}/src/c64dtv
    ${CMAKE_SOURCE_DIR}/src/c128
    ${CMAKE_SOURCE_DIR}/src/cbm2
    ${CMAKE_SOURCE_DIR}/src/pet
    ${CMAKE_SOURCE_DIR}/src/plus4
    ${CMAKE_SOURCE_DIR}/src/scpu64
    ${CMAKE_SOURCE_DIR}/src/vic20
    ${CMAKE_SOURCE_DIR}/src/samplerdrv
    ${CMAKE_SOURCE_DIR}/src/c64/cart
    ${CMAKE_SOURCE_DIR}/src/imagecontents
    ${CMAKE_SOURCE_DIR}/src/monitor
    ${CMAKE_SOURCE_DIR}/src/tapeport
    ${CMAKE_SOURCE_DIR}/src/hvsc
    )

set(HEADLESS_SOURCES
    archdep.c
    joy.c
    kbd.c
    console.c
    ui.c
    uimon.c
    uistatusbar.c
    main.c
    signals.c
    video.c
    vsidui.c
    vsyncarch.c
    c64scui.c
    mousedrv.c
    c64dtvui.c
    scpu64ui.c
    c128ui.c
    vic20ui.c
    petui.c
    plus4ui.c
    cbm2ui.c
    cbm5x0ui.c
    )

add_library(headless STATIC ${HEADLESS_SOURCES})
target_include_directories(headless PRIVATE ${HEADLESS_INCLUDES})

