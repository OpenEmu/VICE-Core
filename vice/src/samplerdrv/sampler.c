/*
 * sampler.c - audio input driver manager.
 *
 * Written by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#include <string.h>

#include "cmdline.h"
#include "file_drv.h"
#include "lib.h"
#include "resources.h"
#include "sampler.h"
#include "translate.h"
#include "util.h"

#ifdef USE_PORTAUDIO
#include "portaudio_drv.h"
#endif

#ifdef USE_PORTAUDIO
#define DEFAULT_DEVICE SAMPLER_DEVICE_PORTAUDIO
#else
#define DEFAULT_DEVICE SAMPLER_DEVICE_FILE
#endif

/* stays at 'DEFAULT_DEVICE' for now, but will become configurable in the future */
static int current_sampler = DEFAULT_DEVICE;
static int sampler_status = SAMPLER_CLOSED;

static sampler_device_t devices[SAMPLER_MAX_DEVICES];

static void sampler_init(void)
{
    memset(devices, 0, sizeof(devices));

    fileaudio_init();

#ifdef USE_PORTAUDIO
    portaudio_init();
#endif
}

/* ------------------------------------------------------------------------- */

void sampler_start(int channels)
{
    if (devices[current_sampler].open) {
        devices[current_sampler].open(channels);
        sampler_status = SAMPLER_STARTED | (channels << 1);
    }
}

void sampler_stop(void)
{
    if (devices[current_sampler].close) {
        devices[current_sampler].close();
        sampler_status = SAMPLER_CLOSED;
    }
}

BYTE sampler_get_sample(int channel)
{
    if (devices[current_sampler].get_sample) {
        return devices[current_sampler].get_sample(channel);
    }
    return 0x80;
}

/* ------------------------------------------------------------------------- */

void sampler_device_register(sampler_device_t *device, int id)
{
    if (id >= SAMPLER_MAX_DEVICES || id < 0) {
        return;
    }

    devices[id].name = device->name;
    devices[id].open = device->open;
    devices[id].close = device->close;
    devices[id].get_sample = device->get_sample;
    devices[id].shutdown = device->shutdown;
    devices[id].resources_init = device->resources_init;
    devices[id].cmdline_options_init = device->cmdline_options_init;
}

/* ------------------------------------------------------------------------- */

static int set_sampler_device(int id, void *param)
{
    int channels;

    /* 1st some sanity checks */
    if (id < 0 || id >= SAMPLER_MAX_DEVICES) {
        return -1;
    }

    /* Nothing changes */
    if (id == current_sampler) {
        return 0;
    }

    /* check if id is registered */
    if (!devices[id].name) {
        return -1;
    }

    if (sampler_status & SAMPLER_STARTED) {
        channels = sampler_status >> 1;
        sampler_stop();
        current_sampler = id;
        sampler_start(channels);
    } else {
        current_sampler = id;
    }

    return 0;
}

static const resource_int_t resources_int[] = {
    { "SamplerDevice", 0, RES_EVENT_NO, (resource_value_t)DEFAULT_DEVICE,
      &current_sampler, set_sampler_device, NULL },
    { NULL }
};

int sampler_resources_init(void)
{
    int i;
    sampler_init();

    for (i = 0; i < SAMPLER_MAX_DEVICES; ++i) {
        if (devices[i].resources_init) {
            if (devices[i].resources_init() < 0) {
                return -1;
            }
        }
    }

    return resources_register_int(resources_int);
}

void sampler_resources_shutdown(void)
{
    int i;

    for (i = 0; i < SAMPLER_MAX_DEVICES; ++i) {
        if (devices[i].shutdown) {
            devices[i].shutdown();
        }
    }
}

/* ------------------------------------------------------------------------- */

static char *cmdline_devices = NULL;


static cmdline_option_t cmdline_options[] =
{
    { "-samplerdev", SET_RESOURCE, 1,
      NULL, NULL, "SamplerDevice", NULL,
      USE_PARAM_ID, USE_DESCRIPTION_COMBO,
      IDGS_DEVICE, IDCLS_SPECIFY_SAMPLER_DEVICE,
      NULL, NULL },
    { NULL }
};

int sampler_cmdline_options_init(void)
{
    int i;
    int started = 0;
    char *temp = NULL;
    char number[4];

    cmdline_devices = lib_stralloc(". (");

    for (i = 0; i < SAMPLER_MAX_DEVICES; ++i) {
        if (devices[i].name) {
            sprintf(number, "%d", i);
            if (!started) {
                temp = util_concat(cmdline_devices, number, ": ", devices[i].name, NULL);
                started = 1;
            } else {
                temp = util_concat(cmdline_devices, ", ", number, ": ", devices[i].name, NULL);
            }
            lib_free(cmdline_devices);
            cmdline_devices = temp;
        }
    }
    temp = util_concat(cmdline_devices, ")", NULL);
    lib_free(cmdline_devices);
    cmdline_devices = temp;

    cmdline_options[0].description = cmdline_devices;

    for (i = 0; i < SAMPLER_MAX_DEVICES; ++i) {
        if (devices[i].cmdline_options_init) {
            if (devices[i].cmdline_options_init() < 0) {
                return -1;
            }
        }
    }

    return cmdline_register_options(cmdline_options);
}
