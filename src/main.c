#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "headers/defvars.h"
#include "headers/colors.h"
#include "headers/trim.h"
#include "headers/get.h"
#include "headers/prompt.h"

extern int errno;

int   g_running = 1;
char  *g_buffer;
char  *g_args[MAXARGS];
char  g_path[PATHLEN];

void command_line_arguments(int argc, char *argv[]){
    if (argc == 1){
        return;
    }

    if (argc > 2){
        fprintf(stderr, "kosmos: too many arguments supplied\n");
        exit(EXIT_FAILURE);
    } else if (!strcmp(argv[1], "-h")){
        printf("\
Usage: kosmos [<option>]\n\n\
Options:\n\
\t-h\tshow this message, then exit\n\
\t-v\tshow the kosmos version number, then exit\n");
        exit(EXIT_SUCCESS);
    } else if (!strcmp(argv[1], "-v")){
        // Version
        printf("Kosmos 0.0.1\n");
        exit(EXIT_SUCCESS);
    }
}

int builtin_exit(){
    exit(EXIT_SUCCESS);
}

int changedir(char *const *args){
    // Implementation of cd
    g_path[0] = '\0';

    // If nothing is given go to the home directory
    if (args[1]){
        // If a path is given copy the string to the path variable
        strcpy(g_path, args[1]);
    } else {
        // Create a path from the "/home/" string and the username
        strcat(g_path, "/home/");
        strcat(g_path, whoami());
    }

    // Change the dir + errors
    if (chdir(g_path))
        fprintf(stderr, "cd: no such directory: %s\n", g_path);

    return 0;
}

// Define the builtin_lookup type
typedef int (*builtin_fpointer)(char *const *args);
struct builtin_lookup{
    char *           name;
    builtin_fpointer func;
};

// Define the contents of the builtins struct
const struct builtin_lookup builtins[] = {
    {"exit", &builtin_exit},
    {"cd", &changedir}};

int builtin(char *const *args){
    for (int i = 0; i < SIZE(builtins); i++){
        if (!strcmp(builtins[i].name, args[0]))
            return builtins[i].func(args);
    }

    return -1;
}

void execute_command(char *const *args){
    int pid;
    int status;

    // Check if the command is a builtin command
    if (builtin(args) >= 0)
        return;

    // Check if the command is empty or null
    if (**args == ' ' || **args == '\0'){
        return;
    // Run the command and return any errors
    } else if ((pid = fork()) == 0){
        int err = execvp(args[0], args);
        if (err == -1){
            // Output errors
            fprintf(stderr, "kosmos: command not found: %s\n", *args);
            exit(EXIT_FAILURE);
        }
        printf("Error: %s\n", strerror(errno));
        exit(errno);
    // If there are no errors wait for it to finish
    } else
        wait(&status);
}

char *create_string(char *str, int len){
    if (str[len - 1] == '\n')
        len--;
    char *newstr = malloc(len + 1);
    memcpy(newstr, str, len);
    newstr[len] = '\0';
    return newstr;
}

char **split_command(char *cmd){
    char *lastptr = cmd;
    int   argcnt  = 0;
    int   len     = 0;

    while (1){
        if (*cmd == ' ' || *cmd == '\0'){
            if (argcnt == MAXARGS){
                fprintf(stderr, "kosmos: too many arguments\n");
                return NULL;
            }
            g_args[argcnt++] = create_string(lastptr, len);

            if (*cmd == '\0'){
                g_args[argcnt] = 0;
                return g_args;
            }

            len     = 0;
            lastptr = ++cmd;
            continue;
        }
        cmd++;
        len++;
    }

    return NULL;
}

void mainloop(){
    char **args = NULL;
    const char *homepath = get_homepath();
    // TAB autocomplete
    rl_bind_key('\t', rl_complete);
    while (1){
        g_buffer = readline(prompt(homepath));
        add_history(g_buffer);

        trim(g_buffer, g_buffer);
        // Split the command
        args = split_command(g_buffer);
        free(g_buffer);
        // Execute the given command
        execute_command(args);
    }
}

int main(int argc,char* argv[]){
    command_line_arguments(argc, argv);
    mainloop();
    return 0;
}
