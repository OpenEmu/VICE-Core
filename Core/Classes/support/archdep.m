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


#import <Foundation/Foundation.h>

#import "archdep.h"
#import "archdep+private.h"

#include "vice.h"
#include "lib.h"
#include "util.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <assert.h>

static char const * boot_path = NULL;

void archdep_shutdown(void)
{
    if (boot_path) {
        lib_free(boot_path);
    }
}

char *archdep_program_name(void)
{
    return lib_stralloc("x64");
}

void archdep_set_boot_path(NSString *path)
{
    boot_path = lib_stralloc([path cStringUsingEncoding:NSASCIIStringEncoding]);
}

const char *archdep_boot_path(void)
{
    return lib_stralloc(boot_path);
}

char *archdep_default_sysfile_pathlist(const char *emu_id)
{
    static char *default_path;
    
    if (default_path == NULL) {
        const char *boot_path;
        
        boot_path = archdep_boot_path();
        
#define MACOSX_ROMDIR "/ROM/"
        
        default_path = util_concat(boot_path, MACOSX_ROMDIR, emu_id, ARCHDEP_FINDPATH_SEPARATOR_STRING,
                                   boot_path, MACOSX_ROMDIR, "DRIVES", ARCHDEP_FINDPATH_SEPARATOR_STRING,
                                   NULL);
    }
    
    return default_path;
}

#pragma mark stubs

char *archdep_default_autostart_disk_image_file_name(void)
{
    const char* home = [NSHomeDirectory() cStringUsingEncoding:NSASCIIStringEncoding];
    return util_concat(home, "/autostart-vice.d64", NULL);
}

char *archdep_default_fliplist_file_name(void)
{
    return lib_stralloc("");
}

int archdep_default_logger(const char *level_string, const char *txt)
{
    return 0;
}

char *archdep_default_resource_file_name(void)
{
    return lib_stralloc("");
}

char *archdep_default_rtc_file_name(void)
{
    return lib_stralloc("");
}

char *archdep_default_save_resource_file_name(void)
{
    return lib_stralloc("");
}

int archdep_expand_path(char **return_path, const char *orig_name)
{
    *return_path = lib_stralloc(orig_name);
    return 0;
}

int archdep_file_is_gzip(const char *name)
{
    return 0;
}

int archdep_file_set_gzip(const char *name)
{
    return 0;
}

char *archdep_filename_parameter(const char *name)
{
    /* nothing special(?) */
    return lib_stralloc(name);
}

char *archdep_get_runtime_cpu(void)
{
    return "CPU";
}

char *archdep_get_runtime_os(void)
{
    return "macos";
}

char *archdep_make_backup_filename(const char *fname)
{
    assert(false);
    
    return lib_stralloc(fname);
}

int archdep_mkdir(const char *pathname, int mode)
{
    return 0;
}

FILE *archdep_mkstemp_fd(char **filename, const char *mode)
{
    assert(false);
    
    return NULL;
}

FILE *archdep_open_default_log_file(void)
{
    return stdout;
}

int archdep_path_is_relative(const char *path)
{
    if (path == NULL) {
        return 0;
    }
    
    return *path != '/';
}

char *archdep_quote_parameter(const char *name)
{
    return lib_stralloc(name);
}

int archdep_rename(const char *oldpath, const char *newpath)
{
    return rename(oldpath, newpath);
}

int archdep_spawn(const char *name, char **argv, char **pstdout_redir, const char *stderr_redir)
{
    assert(false);
    
    return 0;
}

void archdep_startup_log_error(const char *format, ...)
{
    va_list ap;
    
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
}

int archdep_stat(const char *file_name, unsigned int *len, unsigned int *isdir)
{
    struct stat statbuf;
    
    if (stat(file_name, &statbuf) < 0) {
        *len = 0;
        *isdir = 0;
        return -1;
    }
    
    *len = (unsigned int)statbuf.st_size;
    *isdir = S_ISDIR(statbuf.st_mode);
    
    return 0;
}

char *archdep_tmpnam(void)
{
    assert(false);
    
    return NULL;
}

char *archdep_extra_title_text(void)
{
    return NULL;
}
