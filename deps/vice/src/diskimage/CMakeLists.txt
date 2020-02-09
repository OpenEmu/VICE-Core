set(DISKIMAGE_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(DISKIMAGE_SOURCES
    diskimage.c
    fsimage-check.c
    fsimage-check.h
    fsimage-create.c
    fsimage-create.h
    fsimage-dxx.c
    fsimage-dxx.h
    fsimage-gcr.c
    fsimage-gcr.h
    fsimage-p64.c
    fsimage-p64.h
    fsimage-probe.c
    fsimage-probe.h
    fsimage.c
    fsimage.h
    x64.h
    )

add_library(diskimage STATIC ${DISKIMAGE_SOURCES})
target_include_directories(diskimage PRIVATE ${DISKIMAGE_INCLUDES})

