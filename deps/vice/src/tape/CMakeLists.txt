set(TAPE_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/arch/shared
    ${CMAKE_SOURCE_DIR}/src
    )

set(TAPE_SOURCES
    t64.c
    t64.h
    tap.c
    tape-internal.c
    tape-internal.h
    tape-snapshot.c
    tape-snapshot.h
    tape.c
    tapeimage.c
    tapeimage.h
    )

add_library(tape STATIC ${TAPE_SOURCES})
target_include_directories(tape PRIVATE ${TAPE_INCLUDES})

