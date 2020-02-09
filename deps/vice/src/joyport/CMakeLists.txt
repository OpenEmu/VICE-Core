set(JOYPORT_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/samplerdrv
    ${CMAKE_SOURCE_DIR}/src/rtc
    ${CMAKE_SOURCE_DIR}/src/userport
    ${CMAKE_SOURCE_DIR}/src/joyport
    )

set(JOYPORT_SOURCES
    bbrtc.c
    bbrtc.h
    cardkey.c
    cardkey.h
    coplin_keypad.c
    coplin_keypad.h
    cx21.c
    cx21.h
    cx85.c
    cx85.h
    joyport.c
    joyport.h
    joystick.c
    joystick.h
    lightpen.c
    lightpen.h
    mouse.c
    mouse.h
    paperclip64.c
    paperclip64.h
    rushware_keypad.c
    rushware_keypad.h
    sampler2bit.c
    sampler2bit.h
    sampler4bit.c
    sampler4bit.h
    script64_dongle.c
    script64_dongle.h
    snespad.c
    snespad.h
    vizawrite64_dongle.c
    vizawrite64_dongle.h
    waasoft_dongle.c
    waasoft_dongle.h
    )

add_library(joyport STATIC ${JOYPORT_SOURCES})
target_include_directories(joyport PRIVATE ${JOYPORT_INCLUDES})

