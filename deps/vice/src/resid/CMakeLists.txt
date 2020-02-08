set(RESID_INCLUDES
    )

set(RESID_SOURCES
    sid.cc
    voice.cc
    wave.cc
    envelope.cc
    filter.cc
    dac.cc
    extfilt.cc
    pot.cc
    version.cc
    )

set(RESID_BUILT_SOURCES
    wave6581_PST.h
    wave6581_PS_.h
    wave6581_P_T.h
    wave6581__ST.h
    wave8580_PST.h
    wave8580_PS_.h
    wave8580_P_T.h
    wave8580__ST.h
    )

add_library(resid STATIC ${RESID_SOURCES} ${RESID_BUILT_SOURCES})
target_compile_definitions(resid PRIVATE VERSION="3.4")
target_include_directories(resid PRIVATE ${RESID_INCLUDES})
