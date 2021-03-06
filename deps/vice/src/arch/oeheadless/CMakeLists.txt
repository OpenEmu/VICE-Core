add_subdirectory(test)

set(OEHEADLESS_INCLUDES
        ${ARCH_INCLUDES}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/crtc
        ${CMAKE_SOURCE_DIR}/src/drive
        ${CMAKE_SOURCE_DIR}/src/joyport
        ${CMAKE_SOURCE_DIR}/src/lib/p64
        ${CMAKE_SOURCE_DIR}/src/raster
        ${CMAKE_SOURCE_DIR}/src/rs232drv
        ${CMAKE_SOURCE_DIR}/src/sid
        ${CMAKE_SOURCE_DIR}/src/vdc
        ${CMAKE_SOURCE_DIR}/src/c64
        ${CMAKE_SOURCE_DIR}/src/c64dtv
        ${CMAKE_SOURCE_DIR}/src/c128
        ${CMAKE_SOURCE_DIR}/src/cbm2
        ${CMAKE_SOURCE_DIR}/src/pet
        ${CMAKE_SOURCE_DIR}/src/plus4
        ${CMAKE_SOURCE_DIR}/src/scpu64
        ${CMAKE_SOURCE_DIR}/src/vic20
        ${CMAKE_SOURCE_DIR}/src/samplerdrv
        ${CMAKE_SOURCE_DIR}/src/c64/cart
        ${CMAKE_SOURCE_DIR}/src/imagecontents
        ${CMAKE_SOURCE_DIR}/src/monitor
        ${CMAKE_SOURCE_DIR}/src/tapeport
        ${CMAKE_SOURCE_DIR}/src/hvsc
        )

set(OEHEADLESS_SOURCES
        joy.c
        kbd.c
        console.c
        ui.c
        uimon.c
        uistatusbar.c
        vsidui.c
        vsyncarch.c
        c64scui.c
        mousedrv.c
        c64dtvui.c
        scpu64ui.c
        c128ui.c
        vic20ui.c
        petui.c
        plus4ui.c
        cbm2ui.c
        cbm5x0ui.c
        api.m
        ResourceManager.m
        )

set(OEHEADLESS_C64_SOURCES
  C64.m
  C64_archdep.m
  C64_soundheadless.m
  C64_video.m
  )

set(OEHEADLESS_C128_SOURCES
  C128.m
  C128_archdep.m
  C128_soundheadless.m
  C128_video.m
  )

set(OEHEADLESS_VIC20_SOURCES
  Vic20.m
  Vic20_archdep.m
  Vic20_soundheadless.m
  Vic20_video.m
  )

set(OEHEADLESS_ARCHDEP_SOURCES
        archdep_stubs.c
        ../shared/archdep_kbd_get_host_mapping.c
        ../shared/archdep_tmpnam.c
        ../shared/archdep_default_logger.c
        ../shared/archdep_startup_log_error.c
        ../shared/archdep_default_resource_file_name.c
        ../shared/archdep_rename.c
        ../shared/archdep_rmdir.c
        ../shared/archdep_stat.c
        ../shared/archdep_quote_parameter.c
        ../shared/archdep_mkdir.c
        ../shared/archdep_filename_parameter.c
        ../shared/archdep_path_is_relative.c
        ../shared/archdep_mkstemp_fd.c
        ../shared/archdep_open_default_log_file.c
        ../shared/archdep_join_paths.c
        ../shared/archdep_make_backup_filename.c
        ../shared/archdep_extra_title_text.c
        ../shared/archdep_home_path.c
        ../shared/archdep_expand_path.c
        ../shared/archdep_default_fliplist_file_name.c
        ../shared/archdep_default_rtc_file_name.c
        ../shared/archdep_default_autostart_disk_image_file_name.c
        )

add_library(oeheadless_vic20 STATIC ${OEHEADLESS_VIC20_SOURCES})
target_include_directories(oeheadless_vic20 PRIVATE ${OEHEADLESS_INCLUDES})

add_library(oeheadless_c64 STATIC ${OEHEADLESS_C64_SOURCES})
target_include_directories(oeheadless_c64 PRIVATE ${OEHEADLESS_INCLUDES})

add_library(oeheadless_c128 STATIC ${OEHEADLESS_C128_SOURCES})
target_include_directories(oeheadless_c128 PRIVATE ${OEHEADLESS_INCLUDES})

add_library(oeheadless STATIC ${OEHEADLESS_SOURCES} ${OEHEADLESS_ARCHDEP_SOURCES})
target_include_directories(oeheadless PRIVATE ${OEHEADLESS_INCLUDES})
