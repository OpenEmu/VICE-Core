//
//  soundopenemu.c
//  C64C
//
//  Created by Stuart Carnie on 1/12/16.
//  Copyright © 2016 Stuart Carnie. All rights reserved.
//

#include "sound.h"

static int openemu_write(SWORD *pbuf, size_t nr)
{
    return 0;
}

static sound_device_t openemu_device =
{
    "openemu",
    NULL,
    openemu_write,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    2
};

int sound_init_openemu_device(void) {
    return sound_register_device(&openemu_device);
}