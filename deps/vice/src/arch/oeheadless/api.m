#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "archdep.h"
#include "gfxoutput.h"
#include "drive.h"
#include "init.h"
#include "initcmdline.h"
#include "log.h"
#include "machine.h"
#include "maincpu.h"
#include "main.h"
#include "resources.h"
#include "sysfile.h"
#include "uiapi.h"
#include "video.h"
#include "autostart-prg.h"

/* This is the main program entry point.  Call this from `main()'.  */
int main_program(int argc, char **argv)
{
    lib_init_rand();
    
    if (archdep_init(NULL, NULL) != 0) {
        archdep_startup_log_error("archdep_init failed.\n");
        return -1;
    }
    
    maincpu_early_init();
    machine_setup_context();
    drive_setup_context();
    machine_early_init();
    
    /* Initialize system file locator.  */
    sysfile_init(machine_name);
    
    gfxoutput_early_init(0);
    if ((init_resources() < 0) || (init_cmdline_options() < 0)) {
        return -1;
    }
    
    /* Set factory defaults.  */
    if (resources_set_defaults() < 0) {
        archdep_startup_log_error("Cannot set defaults.\n");
        return -1;
    }
    
    if (ui_init(NULL, NULL) < 0) {
        archdep_startup_log_error("Cannot initialize the UI.\n");
        return -1;
    }
    
    /* Load the user's default configuration file.  */
    if (resources_load(NULL) < 0) {
        /* The resource file might contain errors, and thus certain
        resources might have been initialized anyway.  */
        if (resources_set_defaults() < 0) {
            archdep_startup_log_error("Cannot set defaults.\n");
            return -1;
        }
    }
    
    resources_set_int("VICIIDoubleSize", 0);
    resources_set_int("VICIIDoubleScan", 0);
    resources_set_int("VICIIFilter", VIDEO_FILTER_NONE);
    resources_set_int("AutostartPrgMode", AUTOSTART_PRG_MODE_INJECT);
    
    if (log_init() < 0) {
        archdep_startup_log_error("Cannot startup logging system.\n");
    }
    
    /* Complete the GUI initialization (after loading the resources and
       parsing the command-line) if necessary.  */
    if (ui_init_finish() < 0) {
        return -1;
    }
    
    if (video_init() < 0) {
        return -1;
    }
    
    if (initcmdline_check_psid() < 0) {
        return -1;
    }
    
    if (init_main() < 0) {
        return -1;
    }
    
    initcmdline_check_attach();
    
    maincpu_headless_init();
    maincpu_headless_mainloop();
    
    return 0;
}
