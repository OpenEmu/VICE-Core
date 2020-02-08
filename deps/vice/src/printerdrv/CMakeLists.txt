set(PRINTERDRV_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/arch/shared
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/userport
    )

set(PRINTERDRV_SOURCES
    driver-select.c
    driver-select.h
    drv-1520.c
    drv-1520.h
    drv-ascii.c
    drv-ascii.h
    drv-mps803.c
    drv-mps803.h
    drv-nl10.c
    drv-nl10.h
    drv-raw.c
    drv-raw.h
    interface-serial.c
    interface-serial.h
    interface-userport.c
    interface-userport.h
    output-graphics.c
    output-graphics.h
    output-select.c
    output-select.h
    output-text.c
    output-text.h
    output.h
    printer-serial.c
    printer-userport.c
    printer.c
    )

add_library(printerdrv STATIC ${PRINTERDRV_SOURCES})
target_include_directories(printerdrv PRIVATE ${PRINTERDRV_INCLUDES})

