set(SID_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/resid
    ${CMAKE_SOURCE_DIR}/src/resid-dtv
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/joyport
    ${CMAKE_SOURCE_DIR}/src/sid
    )

set(SID_SOURCES
    catweaselmkiii.c
    fastsid.c
    fastsid.h
    hardsid.c
    parsid.c
    sid-cmdline-options.c
    sid-cmdline-options.h
    sid-resources.c
    sid-resources.h
    sid-snapshot.c
    sid-snapshot.h
    sid.c
    sid.h
    ssi2001.c
    wave6581.h
    wave8580.h
    resid.cc
    resid.h
    )

add_library(sid STATIC ${SID_SOURCES})
target_include_directories(sid PRIVATE ${SID_INCLUDES})
