
add_subdirectory(arch)
add_subdirectory(c64)
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
add_subdirectory(vdrive)
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
    log.c
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

set(DRIVER_LIBS joyport samplerdrv sounddrv gfxoutputdrv printerdrv rs232drv diskimage fsdevice tape fileio serial core)

set(X64SC_SOURCES ${SRC_BASE_SOURCES} ${SRC_MIDI_SOURCES})
set(X64SC_LIBS
    tapeport
    c64
    c64cartsystem
    c64cart
    c64commoncart
    driveiec
    driveiecieee
    driveiecc64exp
    driveieee
    drive
    drivetcbm
    iecbus
    p64
    parallel
    vdrive
    sid
    monitor
    ${DRIVER_LIBS}
    ${ARCH_LIBS}
    viciisc
    raster
    userport
    diag
    rtc
    video
    imagecontents
    resid
    )

add_executable(x64sc main.c ${X64SC_SOURCES})

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

target_link_libraries(x64sc ${X64SC_LIBS} z "-framework CoreAudio" "-framework AudioToolbox" "-framework CoreFoundation")
target_compile_options(x64sc PUBLIC "-fobjc-arc")

if(USE_OEHEADLESS)
    add_library(oex64sc SHARED ${X64SC_SOURCES})

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
    target_link_libraries(oex64sc ${X64SC_LIBS} z "-framework Foundation")
    target_compile_options(oex64sc PUBLIC "-fobjc-arc")

    add_custom_command(TARGET oex64sc POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:oex64sc> ${CMAKE_SOURCE_DIR}
            COMMENT "Copying library"
            )

endif()