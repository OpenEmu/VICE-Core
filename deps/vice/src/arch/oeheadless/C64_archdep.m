#import "public/C64.h"
#import "public/api.h"
#import "C64_archdep.h"
#include "vice.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "util.h"
#include "archdep.h"
#include "lib.h"
#include "log.h"

int console_mode = 0;
int video_disabled_mode = 0;


/** \brief  Arch-dependent init
 *
 * \param[in]   argc    pointer to argument count
 * \param[in]   argv    argument vector
 *
 * \return  0
 */
int archdep_init(int *argc, char **argv)
{
    (void)C64.shared;

    return 0;
}

/** \brief  Architecture-dependent shutdown hanlder
 */
void archdep_shutdown(void)
{
}

char *archdep_user_config_path(void)
{
    return (char *)NSHomeDirectory().UTF8String;
}

const char *archdep_boot_path(void)
{
    return lib_strdup(C64.shared.bootPath.UTF8String);
}

char *archdep_default_sysfile_pathlist(const char *emu_id)
{
    const char *paths[17];
    int i = 0;
    
    for (NSString *path in C64.shared.sysfilePathList) {
        paths[i] = path.UTF8String;
        i++;
    }
    
    paths[i] = NULL;
    return util_strjoin(paths, ARCHDEP_FINDPATH_SEPARATOR_STRING);
}

#pragma mark stubs

char *archdep_default_autostart_disk_image_file_name(void)
{
    const char* home = [NSHomeDirectory() cStringUsingEncoding:NSASCIIStringEncoding];
    return util_concat(home, "/autostart-vice.d64", NULL);
}

char *archdep_default_fliplist_file_name(void)
{
    return lib_strdup("");
}

int archdep_default_logger(const char *level_string, const char *txt)
{
    NSLog(@"Vice-Core: %s - %s" , level_string, txt);
    return 0;
}

char *archdep_default_resource_file_name(void)
{
    return lib_strdup("");
}

char *archdep_default_rtc_file_name(void)
{
    return lib_strdup("");
}

char *archdep_default_save_resource_file_name(void)
{
    return lib_strdup("");
}

int archdep_expand_path(char **return_path, const char *orig_name)
{
    *return_path = lib_strdup(orig_name);
    return 0;
}
