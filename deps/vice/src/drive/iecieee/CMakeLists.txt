set(DRIVE_IECIEEE_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(DRIVE_IECIEEE_SOURCES
    iecieee.c
    via2d.c
    )

add_library(driveiecieee STATIC ${DRIVE_IECIEEE_SOURCES})
target_include_directories(driveiecieee PRIVATE ${DRIVE_IECIEEE_INCLUDES})

