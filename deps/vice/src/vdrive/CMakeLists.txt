set(VDRIVE_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/arch/shared
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(VDRIVE_SOURCES
    vdrive-bam.c
    vdrive-bam.h
    vdrive-command.c
    vdrive-command.h
    vdrive-dir.c
    vdrive-dir.h
    vdrive-iec.c
    vdrive-iec.h
    vdrive-internal.c
    vdrive-internal.h
    vdrive-rel.c
    vdrive-rel.h
    vdrive-snapshot.c
    vdrive-snapshot.h
    vdrive.c
    vdrive.h
    )

add_library(vdrive STATIC ${VDRIVE_SOURCES})
target_include_directories(vdrive PRIVATE ${VDRIVE_INCLUDES})

