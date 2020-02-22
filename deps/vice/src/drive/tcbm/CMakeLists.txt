set(DRIVE_TCBM_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(DRIVE_TCBM_SOURCES
    glue1551.c
    glue1551.h
    mem1551.c
    mem1551.h
    tcbm-cmdline-options.c
    tcbm-cmdline-options.h
    tcbm-resources.c
    tcbm-resources.h
    tcbm.c
    tcbmrom.c
    tcbmrom.h
    tpid.c
    tpid.h
    )

add_library(drivetcbm STATIC ${DRIVE_TCBM_SOURCES})
target_include_directories(drivetcbm PRIVATE ${DRIVE_TCBM_INCLUDES})

