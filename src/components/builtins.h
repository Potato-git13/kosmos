#ifndef BUILTINS_HEADER
#define BUILTINS_HEADER

#include "history.h"

void builtin_exit();
void changedir(char *const *args);
void setenvvar(char *const *args);
void free_aliases();
void alias_cmd(char *const *args);
void clear_hist();
void help();
int builtin(char *const *args);
void init_aliases();
void free_aliases();

char g_path[PATHLEN];
int nelements = 0;

struct Aliases **aliases;

void builtin_exit(){
    free_aliases();
    exit(EXIT_SUCCESS);
}

void changedir(char *const *args){
    // Implementation of cd
    const char *homepath = get_homepath();
    g_path[0] = '\0';

    // If nothing is given go to the home directory
    if (args[1]){
        // If a path is given copy the string to the path variable
        strcpy(g_path, args[1]);
    } else {
        // Create a path from the "/home/" string and the username
        strcat(g_path, homepath);
    }

    // Change the dir + errors
    if (chdir(g_path))
        fprintf(stderr, "cd: no such directory: %s\n", g_path);
}

void setenvvar(char *const *args){
    char *envvar = args[1];
    char *value = args[2];

    char *buffer = malloc(strlen(value));

    // Errors
    if(!args[2]){
        fprintf(stderr, "export: not enough arguments\n");
        return;
    }
    if(args[3]){
        fprintf(stderr, "export: too many arguments\n");
        return;
    }

    strcpy(buffer, value);

    // Set the environment variable to the buffer
    int i = setenv(envvar, buffer, 1);

    free(buffer);
    // Errors
    if(i != 0){
        printf("kosmos: %s\n", strerror(errno));
        return;
    }
}

void init_aliases(){
    aliases = calloc(MAX_ALIASES, sizeof(struct Aliases*));
}

void free_aliases(){
    // Free the contents of the aliases struct
    for(int i = 0; i < nelements; i++){
        free(aliases[i]);
    }
}

void alias_cmd(char *const *args){
    // Errors
    if(!args[2] || !args[1]){
        printf("alias: not enough arguments\n");
        return;
    }
    if(args[3]){
        printf("alias: too many arguments\n");
        return;
    }

    // Allocate a new member and copy the command line arguments to it
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
    // Show a help message
    const char help[] = 
        "kosmos, version "VERSION"\n"
        "These commands are defined inside the shell\n"
        "\talias\t\talias [name] [value]\n"
        "\tcd\t\tcd [path]\n"
        "\tclear-history\n"
        "\texit\n"
        "\texport\t\texport [name] [value]\n"
        "\thelp\n"
        ;
        
    printf("%s", help);
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