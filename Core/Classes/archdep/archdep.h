/*
 Copyright (c) 2016, OpenEmu Team
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the OpenEmu Team nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY OpenEmu Team ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL OpenEmu Team BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef archdep_h
#define archdep_h
#include "vice.h"

#define VICE_ARCHAPI_PRIVATE_API
#include "archapi.h"
#undef VICE_ARCHAPI_PRIVATE_API

#include "sound.h"

#include "ui.h"

#define NOT_IMPLEMENTED() \
fprintf(stderr, \
"%s:%d: error: function %s() is not implemented yet, exiting\n", \
__FILE__, __LINE__, __func__); \
exit(1)

/* Video chip scaling.  */
#define ARCHDEP_VICII_DSIZE   1
#define ARCHDEP_VICII_DSCAN   1
#define ARCHDEP_VICII_HWSCALE 1
#define ARCHDEP_VDC_DSIZE     1
#define ARCHDEP_VDC_DSCAN     1
#define ARCHDEP_VDC_HWSCALE   1
#define ARCHDEP_VIC_DSIZE     1
#define ARCHDEP_VIC_DSCAN     1
#define ARCHDEP_VIC_HWSCALE   1
#define ARCHDEP_CRTC_DSIZE    1
#define ARCHDEP_CRTC_DSCAN    1
#define ARCHDEP_CRTC_HWSCALE  1
#define ARCHDEP_TED_DSIZE     1
#define ARCHDEP_TED_DSCAN     1
#define ARCHDEP_TED_HWSCALE   1

/* Video chip double buffering.  */
#define ARCHDEP_VICII_DBUF 0
#define ARCHDEP_VDC_DBUF   0
#define ARCHDEP_VIC_DBUF   0
#define ARCHDEP_CRTC_DBUF  0
#define ARCHDEP_TED_DBUF   0

/* Filesystem dependant operators.  */
#define FSDEVICE_DEFAULT_DIR "."
#define FSDEV_DIR_SEP_STR    "/"
#define FSDEV_DIR_SEP_CHR    '/'
#define FSDEV_EXT_SEP_STR    "."
#define FSDEV_EXT_SEP_CHR    '.'

/* Path separator.  */
#define ARCHDEP_FINDPATH_SEPARATOR_CHAR   ':'
#define ARCHDEP_FINDPATH_SEPARATOR_STRING ":"

/* Modes for fopen().  */
#define MODE_READ              "r"
#define MODE_READ_TEXT         "r"
#define MODE_READ_WRITE        "r+"
#define MODE_WRITE             "w"
#define MODE_WRITE_TEXT        "w"
#define MODE_APPEND            "a"
#define MODE_APPEND_READ_WRITE "a+"

/* Printer default devices.  */
#define ARCHDEP_PRINTER_DEFAULT_DEV1 "print.dump"
#define ARCHDEP_PRINTER_DEFAULT_DEV2 "|lpr"
#define ARCHDEP_PRINTER_DEFAULT_DEV3 "|petlp -F PS|lpr"

/* Default RS232 devices.  */
#define ARCHDEP_RS232_DEV1 "/dev/ttyS0"
#define ARCHDEP_RS232_DEV2 "/dev/ttyS1"
#define ARCHDEP_RS232_DEV3 "rs232.dump"
#define ARCHDEP_RS232_DEV4 "|lpr"

/* Default MIDI devices.  */
#define ARCHDEP_MIDI_IN_DEV  "/dev/midi"
#define ARCHDEP_MIDI_OUT_DEV "/dev/midi"

/* Default location of raw disk images.  */
#define ARCHDEP_RAWDRIVE_DEFAULT "/dev/fd0"

/* Access types */
#define ARCHDEP_R_OK R_OK
#define ARCHDEP_W_OK W_OK
#define ARCHDEP_X_OK X_OK
#define ARCHDEP_F_OK F_OK

/* Standard line delimiter.  */
#define ARCHDEP_LINE_DELIMITER "\n"

/* Ethernet default device */
#define ARCHDEP_ETHERNET_DEFAULT_DEVICE "eth0"


/* Standard line delimiter.  */
#define ARCHDEP_LINE_DELIMITER "\n"


#define ARCHDEP_KEYBOARD_SYM_NONE -1

/* Default sound fragment size */
#define ARCHDEP_SOUND_FRAGMENT_SIZE 2

/* Default sound output mode */
#define ARCHDEP_SOUND_OUTPUT_MODE SOUND_OUTPUT_SYSTEM

const char *archdep_home_path(void);

///* set this path to customize the preference storage */
//const char *archdep_pref_path;

#define LIBDIR VICEDIR

#if defined(__FreeBSD__) || defined(__NetBSD__)
#define DOCDIR PREFIX "/share/doc/vice"
#else
#define DOCDIR LIBDIR "/doc"
#endif

#define VICEUSERDIR ".vice"

/*
 these are used for socket send/recv. in this case we might
 get SIGPIPE if the connection is unexpectedly closed.
 */
void archdep_signals_init(int do_core_dumps);
void archdep_signals_pipe_set(void);
void archdep_signals_pipe_unset(void);

#ifdef MACOSX_SUPPORT
#define MAKE_SO_NAME_VERSION_PROTO(name, version)  "lib" #name "." #version ".dylib"
#else
#define MAKE_SO_NAME_VERSION_PROTO(name, version)  "lib" #name ".so." #version
#endif

/* add second level macro to allow expansion and stringification */
#define ARCHDEP_MAKE_SO_NAME_VERSION(n, v) MAKE_SO_NAME_VERSION_PROTO(n, v)

#ifdef MACOSX_SUPPORT
#define ARCHDEP_OPENCBM_SO_NAME  "libopencbm.dylib"
#define ARCHDEP_LAME_SO_NAME     "libmp3lame.dylib"
#else
#define ARCHDEP_OPENCBM_SO_NAME  "libopencbm.so"
#define ARCHDEP_LAME_SO_NAME     "libmp3lame.so"
#endif

/* what to use to return an error when a socket error happens */
#define ARCHDEP_SOCKET_ERROR errno

/* Keyword to use for a static prototype */
#define STATIC_PROTOTYPE static


#endif /* archdep_h */
