set(DIAG_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    )

set(DIAG_SOURCES
    c64_diag_586220_harness.c
    c64_diag_586220_harness.h
    )

add_library(diag STATIC ${DIAG_SOURCES})
target_include_directories(diag PRIVATE ${DIAG_INCLUDES})

