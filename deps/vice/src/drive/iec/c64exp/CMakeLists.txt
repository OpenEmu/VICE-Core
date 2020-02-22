set(DRIVE_IEC_C64EXP_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/drive/iec
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(DRIVE_IEC_C64EXP_SOURCES
    c64exp-cmdline-options.c
    c64exp-cmdline-options.h
    c64exp-resources.c
    c64exp-resources.h
    iec-c64exp.c
    dolphindos3.c
    dolphindos3.h
    profdos.c
    profdos.h
    stardos-exp.c
    stardos-exp.h
    supercard.c
    supercard.h
    )

add_library(driveiecc64exp STATIC ${DRIVE_IEC_C64EXP_SOURCES})
target_include_directories(driveiecc64exp PRIVATE ${DRIVE_IEC_C64EXP_INCLUDES})
