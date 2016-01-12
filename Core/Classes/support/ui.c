#include "types.h"
#include "uiapi.h"
#include "archdep.h"

// ----- VICEMachineNotifier -----

void archdep_ui_init(int argc, char *argv[])
{
}

int ui_init_finish(void)
{
    return 0;
}

int ui_init_finalize(void)
{
    return 0;
}

int ui_init(int *argc, char **argv)
{
    return 0;
}

void ui_shutdown(void)
{
}

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
    va_list args;
    va_start(args, format);
    va_end(args);
}

void ui_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);
}

ui_jam_action_t ui_jam_dialog(const char *format,...)
{
    va_list args;
    va_start(args, format);
    va_end(args);

    return UI_JAM_NONE;
}

int ui_extend_image_dialog(void)
{
    return 0;
}

char* ui_get_file(const char *format,...)
{
    va_list args;
    va_start(args, format);
    va_end(args);

    return NULL;
}

void ui_dispatch_events()
{
}
