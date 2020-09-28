
add_subdirectory(arch)
add_subdirectory(c64)
add_subdirectory(c64dtv)
add_subdirectory(c128)
add_subdirectory(vic20)
add_subdirectory(core)
add_subdirectory(diag)
add_subdirectory(diskimage)
add_subdirectory(drive)
add_subdirectory(fileio)
add_subdirectory(fsdevice)
add_subdirectory(gfxoutputdrv)
add_subdirectory(iecbus)
add_subdirectory(imagecontents)
add_subdirectory(joyport)
add_subdirectory(lib)
add_subdirectory(mididrv)
add_subdirectory(monitor)
add_subdirectory(parallel)
add_subdirectory(printerdrv)
add_subdirectory(raster)
add_subdirectory(resid)
add_subdirectory(rs232drv)
add_subdirectory(rtc)
add_subdirectory(samplerdrv)
add_subdirectory(serial)
add_subdirectory(sid)
add_subdirectory(sounddrv)
add_subdirectory(tape)
add_subdirectory(tapeport)
add_subdirectory(userport)
add_subdirectory(vdc)
add_subdirectory(vdrive)
add_subdirectory(vicii)
add_subdirectory(viciisc)
add_subdirectory(video)

set(SRC_BUILT_SOURCES
        infocontrib.h
        )

set(SRC_MIDI_SOURCES
        midi.c)

set(SRC_BASE_SOURCES
        alarm.c
        attach.c
        autostart.c
        autostart-prg.c
        cbmdos.c
        cbmimage.c
        charset.c
        clipboard.c
        clkguard.c
        cmdline.c
        color.c
        crc32.c
        datasette.c
        debug.c
        dma.c
        embedded.c
        event.c
        findpath.c
        fliplist.c
        gcr.c
        info.c
        init.c
        initcmdline.c
        interrupt.c
        ioutil.c
        kbdbuf.c
        keyboard.c
        lib.c
        libm_math.c
        $<IF:$<BOOL:${USE_APPLE_LOG}>,log_apple.c,log.c>
        machine-bus.c
        machine.c
        network.c
        opencbmlib.c
        palette.c
        ram.c
        rawfile.c
        rawnet.c
        resources.c
        romset.c
        screenshot.c
        snapshot.c
        socket.c
        sound.c
        sysfile.c
        traps.c
        util.c
        vicefeatures.c
        vsync.c
        zfile.c
        zipcode.c
        )

set(DRIVER_LIBS sounddrv joyport samplerdrv gfxoutputdrv printerdrv rs232drv tape serial core diskimage fsdevice fileio )

set(VICE_SOURCES
        ${SRC_BASE_SOURCES}
        ${SRC_MIDI_SOURCES}
        )

set(COMMON_LIBS
          c64commoncart
          driveiec
          driveiecieee
          driveieee
          drive
          drivetcbm
          iecbus
          p64
          parallel
          vdrive
          sid
          monitor
          video
          raster
          userport
          diag
          rtc
          imagecontents
          resid
        )
set(XVIC20_LIBS
        vic20
        vic20cartsystem
        mascuerade
        oeheadless_vic20
	tapeport
        ${COMMON_LIBS}
        ${ARCH_LIBS}
        ${DRIVER_LIBS}
        )
set(X64_LIBS
        c64base
        c64cart
        c64cartsystem
        driveiecc64exp
	tapeport
        ${COMMON_LIBS}
        ${DRIVER_LIBS}
        ${ARCH_LIBS}
        )

set(X64DTV_LIBS
        c64dtv
        viciidtv
	tapeportdtv
        oeheadless_c64dtv
        driveiecc64exp
        c64c64dtv
        ${COMMON_LIBS}
        ${DRIVER_LIBS}
        ${ARCH_LIBS}
        )

set(X64SC_LIBS
      viciisc
      c64
      oeheadless_c64
      ${X64_LIBS}
  )
set(X128_LIBS
        vicii
        vdc
        c128
        oeheadless_c128
	      driveiec128dcr
        c64c128
        ${X64_LIBS}
)
add_executable(x64sc main.c ${VICE_SOURCES})
add_executable(x64dtv main.c ${VICE_SOURCES} )
add_executable(xvic20 main.c ${VICE_SOURCES})
add_executable(x128 main.c ${VICE_SOURCES})

target_include_directories(x64sc PRIVATE
        ${ARCH_INCLUDES}
        .
        c64
        drive
        monitor
        plus4
        vic20
        vdrive
        diskimage
        imagecontents
        diag
        userport
        lib/p64
        joyport
        rtc
        tapeport
        tape
        socketdrv
        lib/linenoise-ng
        )
target_include_directories(x64dtv PRIVATE
        ${ARCH_INCLUDES}
        .
        c64dtv
        drive
        monitor
        plus4
        vic20
        vdrive
        diskimage
        imagecontents
        diag
        userport
        lib/p64
        joyport
        rtc
        tapeport
        tape
        socketdrv
        lib/linenoise-ng
        )

target_include_directories(x128 PRIVATE
                ${ARCH_INCLUDES}
                .
                c128
                c64
                drive
                monitor
                plus4
                vic20
                vdrive
                vdc
                diskimage
                imagecontents
                diag
                userport
                lib/p64
                joyport
                rtc
                tapeport
                tape
                socketdrv
                lib/linenoise-ng
                )

target_include_directories(xvic20 PRIVATE
            ${ARCH_INCLUDES}
            .
            drive
            monitor
            plus4
            vic20
            vdrive
            vdc
            diskimage
            imagecontents
            diag
            userport
            lib/p64
            joyport
            rtc
            tapeport
            tape
            socketdrv
            lib/linenoise-ng
            )
target_link_libraries(x64sc ${VICE_LIBS} z "-framework CoreAudio" "-framework AudioToolbox" "-framework CoreFoundation")
target_compile_options(x64sc PUBLIC "-fobjc-arc")

target_link_libraries(x64dtv ${VICE_LIBS} z "-framework CoreAudio" "-framework AudioToolbox" "-framework CoreFoundation")
target_compile_options(x64dtv PUBLIC "-fobjc-arc")


target_link_libraries(x128 ${VICE_LIBS} z "-framework CoreAudio" "-framework AudioToolbox" "-framework CoreFoundation")
target_compile_options(x128 PUBLIC "-fobjc-arc")

target_link_libraries(xvic20 ${VICE_LIBS} z "-framework CoreAudio" "-framework AudioToolbox" "-framework CoreFoundation")
target_compile_options(xvic20 PUBLIC "-fobjc-arc")

include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
if (IPO_SUPPORTED)
    set_target_properties(x64sc PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    set_target_properties(x64dtv PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    set_target_properties(x128 PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    set_target_properties(xvic20 PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif ()

if (USE_OEHEADLESS)
    add_library(oex64sc SHARED ${VICE_SOURCES})
    add_library(oex64dtv SHARED ${VICE_SOURCES} ps2mouse.c)
    add_library(oex128 SHARED ${VICE_SOURCES})
    add_library(oexvic20 SHARED ${VICE_SOURCES})

    target_include_directories(oexvic20 PRIVATE
            ${ARCH_INCLUDES}
            .
            drive
            monitor
            plus4
            vic20
            vdrive
            diskimage
            imagecontents
            diag
            userport
            lib/p64
            joyport
            rtc
            tapeport
            tape
            socketdrv
            hvsc
            lib/linenoise-ng
            )

    target_include_directories(oex64sc PRIVATE
            ${ARCH_INCLUDES}
            .
            c64
            drive
            monitor
            plus4
            vic20
            vdrive
            diskimage
            imagecontents
            diag
            userport
            lib/p64
            joyport
            rtc
            tapeport
            tape
            socketdrv
            hvsc
            lib/linenoise-ng
            )

target_include_directories(oex64dtv PRIVATE
        ${ARCH_INCLUDES}
        .
        c64dtv
        drive
        monitor
        vdrive
        diskimage
        imagecontents
        diag
        userport
        lib/p64
        joyport
        tapeport
        tape
        socketdrv
        lib/linenoise-ng
        )

target_include_directories(oex128 PRIVATE
                    ${ARCH_INCLUDES}
                    .
                    c128
                    c64
                    drive
                    monitor
                    plus4
                    vic20
                    vdrive
                    vdc
                    diskimage
                    imagecontents
                    diag
                    userport
                    lib/p64
                    joyport
                    rtc
                    tapeport
                    tape
                    socketdrv
                    hvsc
                    lib/linenoise-ng
                    )
    target_link_libraries(oex64sc ${X64SC_LIBS} z "-framework Foundation" "-framework Carbon")
    target_compile_options(oex64sc PUBLIC "-fobjc-arc")

    target_link_libraries(oex64dtv ${X64DTV_LIBS} z "-framework Foundation" "-framework Carbon")
    target_compile_options(oex64dtv PUBLIC "-fobjc-arc")

    target_link_libraries(oex128 ${X128_LIBS} z "-framework Foundation" "-framework Carbon")
    target_compile_options(oex128 PUBLIC "-fobjc-arc")

    target_link_libraries(oexvic20 ${XVIC20_LIBS} z "-framework Foundation" "-framework Carbon")
    target_compile_options(oexvic20 PUBLIC "-fobjc-arc")

    if (IPO_SUPPORTED)
        set_target_properties(oex64sc PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        set_target_properties(oex64dtv PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        set_target_properties(oex128 PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        set_target_properties(oexvic20 PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    endif ()

    add_custom_command(TARGET oex64sc POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:oex64sc> ${CMAKE_SOURCE_DIR}
            COMMENT "Copying library"
            )

    add_custom_command(TARGET oex64dtv POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:oex64dtv> ${CMAKE_SOURCE_DIR}
            COMMENT "Copying library"
            )

    add_custom_command(TARGET oex128 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:oex128> ${CMAKE_SOURCE_DIR}
            COMMENT "Copying library"
            )
    add_custom_command(TARGET oexvic20 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:oexvic20> ${CMAKE_SOURCE_DIR}
            COMMENT "Copying library"
            )
endif ()
