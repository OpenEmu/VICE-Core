set(DRIVE_IEEE_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(DRIVE_IEEE_SOURCES
    fdc.c
    fdc.h
    ieee-cmdline-options.c
    ieee-cmdline-options.h
    ieee-resources.c
    ieee-resources.h
    ieee.c
    ieeerom.c
    ieeerom.h
    memieee.c
    memieee.h
    riot1d.c
    riot2d.c
    riotd.h
    via1d2031.h
    via1d2031.c
    )

add_library(driveieee STATIC ${DRIVE_IEEE_SOURCES})
target_include_directories(driveieee PRIVATE ${DRIVE_IEEE_INCLUDES})

