set(SERIAL_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(SERIAL_SOURCES
    fsdrive.c
    fsdrive.h
    serial-device.c
    serial-iec-bus.c
    serial-iec-bus.h
    serial-iec-device.c
    serial-iec-device.h
    serial-iec-lib.c
    serial-iec.c
    serial-iec.h
    serial-realdevice.c
    serial-trap.c
    serial-trap.h
    serial.c
    )

add_library(serial STATIC ${SERIAL_SOURCES})
target_include_directories(serial PRIVATE ${SERIAL_INCLUDES})

