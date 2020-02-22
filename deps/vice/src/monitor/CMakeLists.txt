set(MONITOR_INCLUDES
    ${ARCH_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/arch/unix/readline
    ${CMAKE_SOURCE_DIR}/src/drive
    ${CMAKE_SOURCE_DIR}/src/imagecontents
    ${CMAKE_SOURCE_DIR}/src/vdrive
    ${CMAKE_SOURCE_DIR}/src/lib/p64
    )

set(MONITOR_SOURCES
    asm6502.c
    asm6502dtv.c
    asm6809.c
    asmR65C02.c
    asm65816.c
    asmz80.c
    asm.h
    mon_parse.c
    mon_assemble6502.c
    mon_assembleR65C02.c
    mon_assemble65816.c
    mon_assemble6809.c
    mon_assemblez80.c
    mon_assemble.h
    mon_breakpoint.c
    mon_breakpoint.h
    mon_command.c
    mon_command.h
    mon_disassemble.c
    mon_disassemble.h
    mon_drive.c
    mon_drive.h
    mon_file.c
    mon_file.h
    mon_memmap.c
    mon_memmap.h
    mon_memory.c
    mon_memory.h
    mon_register6502.c
    mon_register6502dtv.c
    mon_register6809.c
    mon_registerR65C02.c
    mon_register65816.c
    mon_registerz80.c
    mon_register.h
    mon_register.c
    mon_ui.c
    mon_ui.h
    mon_util.c
    mon_util.h
    mon_lex.c
    monitor.c
    monitor_network.c
    monitor_network.h
    montypes.h
    )

set(MONITOR_BUILT_SOURCES
    mon_parse.c
    mon_parse.h
    )

add_library(monitor STATIC ${MONITOR_SOURCES} ${MONITOR_BUILT_SOURCES})
target_include_directories(monitor PRIVATE ${MONITOR_INCLUDES})

