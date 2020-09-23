
set(VDC_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/vdc
    ${CMAKE_SOURCE_DIR}/src/raster
    ${CMAKE_SOURCE_DIR}/src/drive
    )

set(VDC_SOURCES
    vdc.c
    vdc.h
    vdc-cmdline-options.c
    vdc-cmdline-options.h
    vdc-color.h
    vdc-color.c
    vdc-draw.h
    vdc-draw.c
    vdc-mem.h
    vdc-mem.c
    vdc-resources.c
    vdc-resources.h
    vdc-snapshot.c
    vdc-snapshot.h
    )

add_library(vdc STATIC ${VDC_SOURCES})
target_include_directories(vdc PRIVATE ${VDC_INCLUDES})
