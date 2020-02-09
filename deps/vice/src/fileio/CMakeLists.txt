set(FILEIO_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    )

set(FILEIO_SOURCES
    cbmfile.c
    cbmfile.h
    fileio.c
    p00.c
    p00.h
    )

add_library(fileio STATIC ${FILEIO_SOURCES})
target_include_directories(fileio PRIVATE ${FILEIO_INCLUDES})

