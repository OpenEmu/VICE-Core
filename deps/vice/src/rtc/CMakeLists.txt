set(RTC_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    )

set(RTC_SOURCES
    bq4830y.c
    bq4830y.h
    ds12c887.c
    ds12c887.h
    ds1202_1302.c
    ds1202_1302.h
    ds1216e.c
    ds1216e.h
    ds1307.c
    ds1307.h
    ds1602.c
    ds1602.h
    pcf8583.c
    pcf8583.h
    rtc.c
    rtc.h
    rtc-58321a.c
    rtc-58321a.h
    rtc-72421.c
    rtc-72421.h
    )

add_library(rtc STATIC ${RTC_SOURCES})
target_include_directories(rtc PRIVATE ${RTC_INCLUDES})

