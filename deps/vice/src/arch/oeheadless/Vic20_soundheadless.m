#include "vice.h"

#include <stdio.h>
#import <public/api.h>
#import "Vic20_archdep.h"

#include "sound.h"
#import "Vic20+Private.h"

static int headless_init(const char *param, int *speed, int *fragsize, int *fragnr, int *channels)
{
    [Vic20.shared initSoundSpeed:speed fragSize:fragsize fragNumber:fragnr channels:channels];
    return 0;
}

static int headless_write(int16_t *pbuf, size_t nr)
{
    [Vic20.shared.delegate updateAudioBuffer:pbuf samples:nr];
    return 0;
}

static sound_device_t headless_device =
        {
                .name = "headless",
                .init = headless_init,
                .write = headless_write,
                .need_attenuation = 0,
                .max_channels = 2,
        };

int sound_init_headless_device(void)
{
    return sound_register_device(&headless_device);
}
