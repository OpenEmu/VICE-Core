//
// Created by Stuart Carnie on 2/8/20.
//

#include <assert.h>

void archdep_signals_init(int do_core_dumps)
{}

void archdep_vice_exit(int excode)
{}

void signals_init(int do_core_dumps)
{}

int archdep_vice_atexit(void (*function)(void))
{
    return 0;
}

int archdep_spawn(const char *name, char **argv,
                  char **pstdout_redir, const char *stderr_redir)
{
    assert(0);
    return -1;
}