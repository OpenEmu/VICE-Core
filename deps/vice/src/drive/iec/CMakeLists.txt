add_subdirectory(c64exp)

set(DRIVE_IEC_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(DRIVE_IEC_SOURCES
    cia1571d.c
    cia1581d.c
    ciad.h
    fdd.c
    fdd.h
    glue1571.c
    glue1571.h
    iec-cmdline-options.c
    iec-cmdline-options.h
    iec-resources.c
    iec-resources.h
    iec.c
    iecrom.c
    iecrom.h
    memiec.c
    memiec.h
    pc8477.c
    pc8477.h
    via1d1541.c
    via1d1541.h
    via4000.c
    via4000.h
    wd1770.c
    wd1770.h
    )

add_library(driveiec STATIC ${DRIVE_IEC_SOURCES})
target_include_directories(driveiec PRIVATE ${DRIVE_IEC_INCLUDES})

