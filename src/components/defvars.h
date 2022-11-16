#ifndef DEFVARS_HEADER
#define DEFVARS_HEADER

// Variable definitions
#define VERSION "0.1.5"
#define MAXARGS 32
#define COMMANDLEN 1024
#define PATHLEN 256
#define HOST_NAME_MAX 64
#define MAX_ALIASES 512
#define MAX_LINES 1024

#define HISTSIZE 3000
#define SAVEHIST 3000

struct Aliases{
    char substring[COMMANDLEN];
    char replace[COMMANDLEN];
};


#define SIZE(x) (sizeof x / sizeof x[0])

#endif
