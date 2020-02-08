set(LIB_P64_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    )

set(LIB_P64_SOURCES
    p64.c)

add_library(p64 STATIC ${LIB_P64_SOURCES})
target_include_directories(p64 PRIVATE ${LIB_P64_INCLUDES})

