#import "public/api.h"

#include "vice.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "archdep.h"
#include "lib.h"
#include "log.h"

#include "../shared/archdep_create_user_config_dir.h"

/** \brief  Reference to argv[0]
 *
 * FIXME: this is only used twice I think, better pass this as an argument to
 *        the functions using it
 */
static char *argv0 = NULL;

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
    argv0 = lib_strdup(argv[0]);

    /* set argv0 for program_name()/boot_path() calls (yes, not ideal) */
    archdep_program_path_set_argv0(argv[0]);

    archdep_create_user_config_dir();

    /* needed for early log control (parses for -silent/-verbose) */
    log_verbose_init(*argc, argv);

    return 0;
}

/** \brief  Architecture-dependent shutdown hanlder
 */
void archdep_shutdown(void)
{
/* free memory used by the exec path */
    archdep_program_path_free();
    /* free memory used by the exec name */
    archdep_program_name_free();
    /* free memory used by the boot path */
    archdep_boot_path_free();
    /* free memory used by the home path */
    archdep_home_path_free();
    /* free memory used by the config files path */
    archdep_user_config_path_free();
    /* free memory used by the sysfile pathlist */
    archdep_default_sysfile_pathlist_free();

    /* this should be removed soon */
    if (argv0 != NULL) {
        lib_free(argv0);
        argv0 = NULL;
    }
}

