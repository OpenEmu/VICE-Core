set(DRIVE_IEC128DCR_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(DRIVE_IEC128DCR_SOURCES
  iec128dcr-cmdline-options.c
  iec128dcr-cmdline-options.h
  iec128dcr-resources.c
  iec128dcr-resources.h
  iec128dcr.c
  iec128dcrrom.c
  iec128dcrrom.h
    )

add_library(driveiec128dcr STATIC ${DRIVE_IEC128DCR_SOURCES})
target_include_directories(driveiec128dcr PRIVATE ${DRIVE_IEC128DCR_INCLUDES})
