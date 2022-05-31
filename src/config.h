#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#define HISTSIZE 3000
#define SAVEHIST 3000

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
