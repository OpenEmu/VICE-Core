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


#import "main_program.h"

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
#include "vicii.h"
#include "interrupt.h"
#include "sid/sid.h"
//#include "c64/cart/c64cartsystem.h"

#include <stdbool.h>
#include <assert.h>

// from main.c
int console_mode = 0;
int video_disabled_mode = 0;

static int init_c64()
{
    if(init_main() < 0)
    {
        assert(false);
    }
    
    // configure vice
    resources_set_int("SoundFragmentSize", SOUND_FRAGMENT_MEDIUM);
    resources_set_int("VICIIDoubleSize", 0);
    resources_set_int("VICIIFilter", VIDEO_FILTER_NONE);

    return 0;
}

/** OpenEmu replacement for main_program
 */
int main_init()
{
    
    maincpu_early_init();
    machine_setup_context();
    drive_setup_context();
    machine_early_init();
    
    /* Initialize system file locator.  */
    sysfile_init(machine_name);
    
    if(init_resources() < 0)
    {
        assert(false);
    }
    
    // Set factory defaults
    if(resources_set_defaults() < 0)
    {
        assert(false);
    }
    
    /*if(log_init() < 0)
     {
        assert(false);
     }*/
    
    init_c64();

    return 0;
}


void main_exit()
{
}
