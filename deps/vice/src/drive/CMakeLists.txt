add_subdirectory(iec)
add_subdirectory(iecieee)
add_subdirectory(ieee)
add_subdirectory(tcbm)

set(DRIVE_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/vdrive
    ${CMAKE_SOURCE_DIR}/src/rtc
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    iec
    tcbm
    ieee
    )

set(DRIVE_SOURCES
    drive-check.c
    drive-check.h
    drive-cmdline-options.c
    drive-cmdline-options.h
    drive-overflow.c
    drive-overflow.h
    drive-resources.c
    drive-resources.h
    drive-snapshot.c
    drive-snapshot.h
    drive-sound.c
    drive-sound.h
    drive-writeprotect.c
    drive-writeprotect.h
    drive.c
    drive.h
    drivecpu.c
    drivecpu.h
    drivecpu65c02.c
    drivecpu65c02.h
    driveimage.c
    driveimage.h
    drivemem.c
    drivemem.h
    driverom.c
    driverom.h
    drivesync.c
    drivesync.h
    drivetypes.h
    iec-c64exp.h
    iec-plus4exp.h
    iec.h
    iec128dcr.h
    iecieee.h
    ieee.h
    rotation.c
    rotation.h
    tcbm.h
    viad.h
    )

add_library(drive STATIC ${DRIVE_SOURCES})
target_include_directories(drive PRIVATE ${DRIVE_INCLUDES})

