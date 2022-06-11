#ifndef BUILTINS_HEADER
#define BUILTINS_HEADER

#include "../config.h"
#include "history.h"

char g_path[PATHLEN];
int nelements;

struct Aliases **aliases;
void free_aliases();

void builtin_exit(){
    free_aliases();
    exit(EXIT_SUCCESS);
}

void changedir(char *const *args){
    // Implementation of cd
    g_path[0] = '\0';
    char *temp = malloc(sizeof(g_path) * sizeof(char*));

    // If nothing is given go to the home directory
    if (args[1]){
        // If a path is given copy the string to the path variable
        strcpy(g_path, args[1]);
    } else {
        // Create a path from the "/home/" string and the username
        strcat(g_path, "/home/");
        strcat(g_path, whoami());
    }

    if (g_path[0] == '~'){
        memmove(&g_path[0], &g_path[1], strlen(g_path) - 0);
        strcpy(temp, g_path);

        strcpy(g_path, "/home/");
        strcat(g_path, whoami());
        strcat(g_path, temp);
    }
    free(temp);

    // Change the dir + errors
    if (chdir(g_path))
        fprintf(stderr, "cd: no such directory: %s\n", g_path);
}

void setenvvar(char *const *args){
    if(!args[2]){
        fprintf(stderr, "export: not enough arguments\n");
        return;
    }
    if(args[3]){
        fprintf(stderr, "exprt: too many arguments\n");
    }

    char *envvar = args[1];
    char *value = args[2];

    char *env_buffer = getenv(envvar);
    char *buffer = malloc(sizeof(env_buffer) + PATHLEN);

    strcpy(buffer, value);
    if(env_buffer){
        strcat(buffer, ":");
        strcat(buffer, env_buffer);
    };

    int i = setenv(envvar, buffer, 1);
    if(i != 0){
        printf("kosmos: %s\n", strerror(errno));
        free(buffer);
        return;
    }

    free(buffer);
}

void init_aliases(){
    aliases = calloc(MAX_ALIASES, sizeof(struct Aliases*));
}

void free_aliases(){
    for(int i = 0; i < nelements; i++){
        free(aliases[i]);
    }
}

void alias_cmd(char *const *args){
    if(!args[2]){
        printf("alias: not enough arguments\n");
        return;
    }
    if(args[3]){
        printf("alias: too many arguments\n");
    }

    aliases[nelements] = calloc(1, sizeof(struct Aliases));
    strcpy(aliases[nelements]->substring, args[1]);
    strcpy(aliases[nelements]->replace, args[2]);
    nelements++;
}

void clear_hist(){
    char *homepath = get_homepath();
    char *hist_file = malloc(sizeof(homepath) + sizeof("/.kosmos_history") * sizeof(char *));
    create_hist_name(homepath, hist_file);

    // Open the file for writing and close it - clearing it
    FILE *fp = fopen(hist_file, "w");
    fclose(fp);
    // Clear the history in memory
    rl_clear_history();

    free(homepath);
    free(hist_file);
}

void help(){
    printf("\
kosmos, version %s\n\
These commands are defined inside the shell\n\n\
\
alias - alias NAME VALUE\n\
cd - cd PATH\n\
clear-history\n\
exit\n\
export - export NAME VALUE\n", VERSION);
}

// Define the builtins struct
typedef void (*builtin_function_pointer)(char *const *args);
struct builtins{
    char *                   name;
    builtin_function_pointer func;
};

// Define the contents of the builtins struct
const struct builtins builtins[] = {
    {"exit", &builtin_exit},
    {"cd", &changedir},
    {"export", &setenvvar},
    {"alias", &alias_cmd},
    {"clear-history", &clear_hist},
    {"help", &help}
};

int builtin(char *const *args){
    // If the arguments given are the same as some builtin commands' name call them
    for (int i = 0; i < (int)SIZE(builtins); i++){
        if (!strcmp(builtins[i].name, args[0])){
            builtins[i].func(args);
            return 0;
        }
    }

    return -1;
}

#endif