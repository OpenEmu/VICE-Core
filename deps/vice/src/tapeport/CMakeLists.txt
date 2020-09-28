 set(TAPEPORT_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/rtc
    ${CMAKE_SOURCE_DIR}/src/diag
    )

set(TAPEPORTDTV_SOURCES
    cp-clockf83.c
    cp-clockf83.h
    dtl-basic-dongle.c
    dtl-basic-dongle.h
    sense-dongle.c
    sense-dongle.h
    tape_diag_586220_harness.c
    tape_diag_586220_harness.h
    tapecart.c
    tapecart.h
    tapecart-loader.h
    tapelog.c
    tapelog.h
    tapeport.h
    )

set(TAPEPORT_SOURCES
	${TAPEPORTDTV_SOURCES}
	tapeport.c
	)

add_library(tapeportdtv STATIC ${TAPEPORTDTV_SOURCES})
target_include_directories(tapeportdtv PRIVATE ${TAPEPORT_INCLUDES})

add_library(tapeport STATIC ${TAPEPORT_SOURCES})
target_include_directories(tapeport PRIVATE ${TAPEPORT_INCLUDES})

