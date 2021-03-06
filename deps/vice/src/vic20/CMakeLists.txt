add_subdirectory(cart)

set(VIC20_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/vic20/cart
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/raster
    ${CMAKE_SOURCE_DIR}/src/sid
    ${CMAKE_SOURCE_DIR}/src/tape
    ${CMAKE_SOURCE_DIR}/src/userport
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    ${CMAKE_SOURCE_DIR}/src/video
    ${CMAKE_SOURCE_DIR}/src/joyport
    ${CMAKE_SOURCE_DIR}/src/samplerdrv
    ${CMAKE_SOURCE_DIR}/src/tapeport
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/rs232drv
    )

set(VIC20_SOURCES
      vic-cmdline-options.c
      vic-cmdline-options.h
      vic-color.c
      vic-color.h
      vic-cycle.c
      vic-cycle.h
      vic-draw.c
      vic-draw.h
      vic-mem.c
      vic-mem.h
      vic-resources.c
      vic-resources.h
      vic-snapshot.c
      vic-snapshot.h
      vic-timing.c
      vic-timing.h
      vic.c
      vic.h
      victypes.h
      vic20-cmdline-options.c
      vic20-cmdline-options.h
      vic20-resources.c
      vic20-resources.h
      vic20-snapshot.c
      vic20-snapshot.h
      vic20.c
      vic20.h
      vic20bus.c
      vic20cpu.c
      vic20datasette.c
      vic20drive.c
      vic20embedded.c
      vic20export.c
      vic20iec.c
      vic20iec.h
      vic20ieeevia.h
      vic20ieeevia1.c
      vic20ieeevia2.c
      vic20io.c
      vic20mem.c
      vic20mem.h
      vic20memrom.h
      vic20memrom.c
      vic20memsnapshot.c
      vic20memsnapshot.h
      vic20model.c
      vic20model.h
      vic20printer.c
      vic20rom.c
      vic20rom.h
      vic20romset.c
      vic20rsuser.c
      vic20rsuser.h
      vic20sound.c
      vic20sound.h
      vic20via.h
      vic20via1.c
      vic20via2.c
      vic20video.c
      )

add_library(vic20 STATIC ${VIC20_SOURCES})
target_include_directories(vic20 PRIVATE ${VIC20_INCLUDES})
