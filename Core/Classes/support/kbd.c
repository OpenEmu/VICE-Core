#include <stdio.h>
#include <stdlib.h>

#include "vice.h"
#include "kbd.h"
#include "keyboard.h"

void kbd_arch_init(void)
{
}

signed long kbd_arch_keyname_to_keynum(char *keyname)
{
    return (signed long)atoi(keyname);
}

const char *kbd_arch_keynum_to_keyname(signed long keynum)
{
    static char keyname[20];
    sprintf(keyname, "%li", keynum);
    return keyname;
}

void kbd_initialize_numpad_joykeys(int* joykeys)
{
}

int kbd_arch_get_host_mapping(void)
{
    /* FIXME: determine layout */
    return KBD_MAPPING_US;
}
