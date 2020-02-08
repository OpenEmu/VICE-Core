set(PARALLEL_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/arch/shared
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(PARALLEL_SOURCES
    parallel-trap.c
    parallel-trap.h
    parallel.c
    )

add_library(parallel STATIC ${PARALLEL_SOURCES})
target_include_directories(parallel PRIVATE ${PARALLEL_INCLUDES})

