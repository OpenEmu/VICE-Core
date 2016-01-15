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


#include <stdlib.h>
#include "types.h"
#include "uiapi.h"
#include "archdep.h"
#include "vice.h"
#include "cmdline.h"
#include "resources.h"
#include "sound.h"


// ----- Drive Status Display -----

void ui_enable_drive_status(ui_drive_enable_t enable, int * color)
{
}

void ui_display_drive_track(unsigned int drive_number, unsigned int drive_base,
                            unsigned int half_track_number)
{
}

void ui_display_drive_led(int drive_number, unsigned int pwm1, unsigned int pwm2)
{
}

void ui_display_drive_current_image(unsigned int drive_number,
                                    const char *image)
{
}

// ----- Tape Status & Control -----

void ui_set_tape_status(int enable)
{
}

void ui_display_tape_current_image(const char *image)
{
}

void ui_display_tape_control_status(int control)
{
}

void ui_display_tape_motor_status(int motor)
{
}

void ui_display_tape_counter(int counter)
{
}

// ---------- Status Messages ----------

void ui_display_speed(float speed, float frame_rate, int warp_enabled)
{
}

// Event Recording/Playback

void ui_display_recording(int recording_status)
{
}

void ui_display_playback(int playback_status, char *version)
{
}

void ui_display_event_time(unsigned int current, unsigned int total)
{
}

void ui_display_statustext(const char *text, int fade_out)
{
}

void ui_display_joyport(BYTE *joyport)
{
}

void ui_display_volume(int vol)
{
}

void ui_update_menus()
{
}

// ----- Alerts & Dialogs -----

void ui_error(const char *format, ...)
{
}

void ui_message(const char *format, ...)
{
}

ui_jam_action_t ui_jam_dialog(const char *format,...)
{
    return UI_JAM_NONE;
}

int ui_extend_image_dialog(void)
{
    return 0;
}

void ui_dispatch_events()
{
}

