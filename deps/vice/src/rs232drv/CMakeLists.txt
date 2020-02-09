set(RS232DRV_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/rs232drv
    ${CMAKE_SOURCE_DIR}/src
    )

set(RS232DRV_SOURCES
    rs232dev.h
    rs232.c
    rs232.h
    rs232drv.c
    rs232drv.h
    rs232net.c
    rs232net.h
    rsuser.c
    rsuser.h
    )

add_library(rs232drv STATIC ${RS232DRV_SOURCES})
target_include_directories(rs232drv PRIVATE ${RS232DRV_INCLUDES})
