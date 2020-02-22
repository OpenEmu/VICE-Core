set(IECBUS_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(IECBUS_SOURCES
    iecbus.c
    )

add_library(iecbus STATIC ${IECBUS_SOURCES})
target_include_directories(iecbus PRIVATE ${IECBUS_INCLUDES})
