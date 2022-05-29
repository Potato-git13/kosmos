#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include "headers/defvars.h"

struct Aliases{
    char substring[COMMANDLEN];
    char replace[COMMANDLEN];
};

const struct Aliases aliases[] = {
    {"ls", "ls --color"},
    {"grep", "grep --color"},
};

#endif
