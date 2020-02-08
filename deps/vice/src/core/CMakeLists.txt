set(CORE_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/arch/shared
    ${CMAKE_SOURCE_DIR}/src
    )

set(CORE_SOURCES
    ata.c
    ata.h
    ciacore.c
    ciatimer.c
    ciatimer.h
    cs8900.c
    cs8900.h
    flash040core.c
    fmopl.c
    fmopl.h
    mc6821core.c
    mc6821core.h
    m93c86.c
    m93c86.h
    riotcore.c
    ser-eeprom.c
    ser-eeprom.h
    spi-sdcard.c
    spi-sdcard.h
    t6721.c
    t6721.h
    tpicore.c
    viacore.c
    )

add_library(core STATIC ${CORE_SOURCES})
target_include_directories(core PRIVATE ${CORE_INCLUDES})

