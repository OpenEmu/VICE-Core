set(USERPORT_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/c64/cart
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    ${CMAKE_SOURCE_DIR}/src/rtc
    ${CMAKE_SOURCE_DIR}/src/joyport
    ${CMAKE_SOURCE_DIR}/src/samplerdrv
    ${CMAKE_SOURCE_DIR}/src/diag
    )

set(USERPORT_SOURCES
    userport.c
    userport.h
    userport_4bit_sampler.c
    userport_4bit_sampler.h
    userport_8bss.c
    userport_8bss.h
    userport_dac.c
    userport_dac.h
    userport_diag_586220_harness.c
    userport_diag_586220_harness.h
    userport_digimax.c
    userport_digimax.h
    userport_joystick.c
    userport_joystick.h
    userport_rtc_58321a.c
    userport_rtc_58321a.h
    userport_rtc_ds1307.c
    userport_rtc_ds1307.h
    )

add_library(userport STATIC ${USERPORT_SOURCES})
target_include_directories(userport PRIVATE ${USERPORT_INCLUDES})

