#ifndef CONFIG_HEADER
#define CONFIG_HEADER

struct Aliases{
    char substring[COMMANDLEN];
    char replace[COMMANDLEN];
};

// Basic alias examples
const struct Aliases aliases[] = {
    {"ls", "ls --color"},
    {"grep", "grep --color"},
};

#endif
