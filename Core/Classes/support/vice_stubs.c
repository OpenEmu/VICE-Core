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


#include "machine.h"
#include "maincpu.h"
#include "drive.h"
#include "lib.h"
#include "util.h"
#include "ioutil.h"
#include "uiapi.h"
#include "console.h"
#include "monitor.h"
#include "video.h"
#include "cmdline.h"
#include "gfxoutput.h"
#include "videoarch.h"
#include "init.h"
#include "resources.h"
#include "sysfile.h"
#include "log.h"
#include "archdep.h"
#include "palette.h"
#include "c64model.h"
#include "keyboard.h"
#include "autostart.h"
#include "kbdbuf.h"
#include "attach.h"
#include "raster.h"
#include "viciitypes.h"
#include "sound.h"
#include "cartridge.h"
#include "tape.h"
#include "viewport.h"

#define CLINK

#pragma mark uimon stubs

CLINK int uimon_out(const char *buffer) { return 0; }
CLINK char *uimon_get_in(char **ppchCommandLine, const char *prompt)
{
    static char buff[] = "";
    return buff;
}
CLINK void uimon_notify_change(void) { }
CLINK console_t *uimon_window_resume(void) { return NULL; }
CLINK void uimon_window_close(void) { }
CLINK console_t *uimon_window_open(void) { return NULL; }
CLINK void uimon_set_interface(monitor_interface_t **monitor_interface_init, int count) { }
CLINK void uimon_window_suspend(void) { }


#pragma mark fullscreen stubs

CLINK void fullscreen_resume(void) { }
CLINK void fullscreen_capability(cap_fullscreen_t *cap_fullscreen) { }


#pragma mark c64ui

int c64ui_init(void) { return 0; }
void c64ui_shutdown(void) { }


#pragma mark ui_display stubs

#if 0
CLINK void ui_display_tape_current_image(const char *image) { }
CLINK void ui_display_drive_current_image(unsigned int drive_number, const char *image) { }
CLINK void ui_display_tape_control_status(int control) { }
CLINK void ui_display_tape_motor_status(int motor) { }
CLINK void ui_display_tape_counter(int counter) { }
CLINK void ui_display_recording(int recording_status) { }
CLINK void ui_display_playback(int playback_status, char *version) { }
CLINK void ui_display_event_time(unsigned int current, unsigned int total) { }
CLINK void ui_display_volume(int vol) { }

CLINK void ui_set_tape_status(int tape_status) { }
CLINK ui_jam_action_t ui_jam_dialog(const char *format, ...) { return UI_JAM_NONE; }
#endif


#pragma mark ui stubs

CLINK int ui_init_finalize() { return 0; }
CLINK void ui_shutdown(void) { }
CLINK char* ui_get_file(const char *format,...) { return NULL; }
CLINK int ui_cmdline_options_init() { return 0; }
CLINK int ui_resources_init() { return 0; }
CLINK void ui_resources_shutdown(void) { }
CLINK void ui_cmdline_show_help(unsigned int num_options, cmdline_option_ram_t *options, void *userparam) { }


#pragma mark kbd stubs

#if 0
CLINK int c64_kbd_init(void) { return 0; }
CLINK void kbd_arch_init(void) { }
CLINK void kbd_initialize_numpad_joykeys(int* joykeys) { }
CLINK int kbd_cmdline_options_init(void) { return 0; }
CLINK int kbd_resources_init(void) { return 0; }
#endif


#pragma mark joystick stubs

#if 0
CLINK int joystick_arch_init_resources(void) { return 0; }
CLINK void joy_arch_init_default_mapping(int joynum) { }
CLINK int joystick_init_resources(void) { return 0; }
#endif
int joy_arch_resources_init(void) { return 0; }
int joy_arch_cmdline_options_init(void) { return 0; }
int joy_arch_init(void) { return 0; }
void joystick_close(void) { }
int joy_arch_set_device(int port, int new_dev) { return 0; }


#pragma mark gfxoutput stubs

CLINK gfxoutputdrv_t *gfxoutput_get_driver(const char *drvname) { return NULL; }
CLINK void gfxoutput_shutdown(void) { }
CLINK int gfxoutput_resources_init() { return 0; }
CLINK int gfxoutput_init() { return 0; }
CLINK int gfxoutput_early_init(int help) { return 0; }
CLINK int gfxoutput_cmdline_options_init() { return 0; }


#pragma mark video stubs

CLINK int video_arch_resources_init(void) { return 0; }
CLINK void video_arch_resources_shutdown(void) { }
int video_arch_cmdline_options_init(void) { return 0; }
int video_init_cmdline_options(void) { return 0; }

#if 0
CLINK char video_canvas_can_resize(video_canvas_t *canvas) { return 1; }
CLINK void video_canvas_destroy(struct video_canvas_s *canvas) { }
CLINK void video_shutdown(void) { }
#endif

#pragma mark signals stubs

CLINK int signals_init(int do_core_dumps) { return 0; }


#pragma mark console stubs

CLINK int console_init() { return 0; }
CLINK int console_close_all(void) { return 0; }


#pragma mark screenshot stubs

#if 0
CLINK int screenshot_init() { return 0; }
CLINK int screenshot_record() { return 0; }
CLINK int screenshot_save(const char *drvname, const char *filename, struct video_canvas_s *canvas) { return 0; }
CLINK void screenshot_prepare_reopen() { }
CLINK void screenshot_try_reopen() { }
#endif


#pragma mark sysfile stubs

#if 0
CLINK int sysfile_init(const char *emu_id) { return 0; }
CLINK void sysfile_shutdown() { }
CLINK int sysfile_resources_init() { return 0; }
CLINK void sysfile_resources_shutdown() { }
CLINK int sysfile_cmdline_options_init() { return 0; }
#endif


#pragma mark cmdline stubs

#if 0
CLINK int cmdline_register_options(const cmdline_option_t *c) { return 0; }
CLINK int cmdline_init() { return 0; }
CLINK void cmdline_shutdown() { }
CLINK int initcmdline_init() { return 0; }
#endif
