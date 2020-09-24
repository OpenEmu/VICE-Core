set(CART_INCLUDES
      ${ARCH_INCLUDES}
      ${CMAKE_SOURCE_DIR}/src
      ${CMAKE_SOURCE_DIR}/src/vic20
      ${CMAKE_SOURCE_DIR}/src/c64/cart
      ${CMAKE_SOURCE_DIR}/src/raster
      ${CMAKE_SOURCE_DIR}/src/sid
      ${CMAKE_SOURCE_DIR}/src/lib/p64
      )

set(VIC20CARTSYSTEM_SOURCES
      behrbonz.c
      behrbonz.h
      debugcart.c
      debugcart.h
      finalexpansion.c
      finalexpansion.h
      ioramcart.c
      ioramcart.h
      megacart.c
      megacart.h
      ultimem.c
      ultimem.h
      vic-fp.c
      vic-fp.h
      vic20-generic.c
      vic20-generic.h
      vic20-ieee488.c
      vic20-ieee488.h
      vic20-midi.c
      vic20-midi.h
      vic20-sidcart.c
      vic20cart.c
      vic20cart.h
      vic20cartmem.c
      vic20cartmem.h
    )

  set(MASCUERADE_SOURCES
    	mascuerade-stubs.h
    	mascuerade-stubs.c
      )

add_library(vic20cartsystem STATIC ${VIC20CARTSYSTEM_SOURCES})
add_library(mascuerade STATIC ${MASCUERADE_SOURCES})

target_include_directories(vic20cartsystem PRIVATE ${CART_INCLUDES})
target_include_directories(mascuerade PRIVATE ${CART_INCLUDES})
