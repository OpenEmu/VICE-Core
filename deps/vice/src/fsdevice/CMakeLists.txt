set(FSDEVICE_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/vdrive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(FSDEVICE_SOURCES
    fsdevice-close.c
    fsdevice-close.h
    fsdevice-cmdline-options.c
    fsdevice-cmdline-options.h
    fsdevice-flush.c
    fsdevice-flush.h
    fsdevice-open.c
    fsdevice-open.h
    fsdevice-read.c
    fsdevice-read.h
    fsdevice-resources.c
    fsdevice-resources.h
    fsdevice-write.c
    fsdevice-write.h
    fsdevice.c
    fsdevicetypes.h
    )

add_library(fsdevice STATIC ${FSDEVICE_SOURCES})
target_include_directories(fsdevice PRIVATE ${FSDEVICE_INCLUDES})
