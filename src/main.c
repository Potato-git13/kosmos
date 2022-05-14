#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "headers/colors.h"
#include "headers/trim.h"
#include "headers/get.h"

extern int errno;

#define MAXARGS 32
#define PATHLEN 256
#define HOST_NAME_MAX 64
#define COMMANDLEN 1024
#define SIZE(x) (sizeof x / sizeof x[0])

int   g_running = 1;
char  *g_buffer;
char  g_curpath[PATHLEN];
char  *g_args[MAXARGS];
char  path[PATHLEN];
char shell_prompt[512];

void command_line_arguments(int arg_counter, char *argv[]){
    if (arg_counter == 1){
        return;
    }

    if (arg_counter > 2){
        printf("kosmos: too many arguments supplied\n");
        exit(0);
    } else if (!strcmp(argv[1], "-h")){
        printf("\
Usage: kosmos [<option>]\n\n\
Options:\n\
\t-h\tshow this message, then exit\n\
\t-v\tshow the kosmos version number, then exit\n");
        exit(0);
    } else if (!strcmp(argv[1], "-v")){
        // Version
        printf("Kosmos 0.0.1\n");
        exit(0);
    }
}

int builtin_exit(){
    exit(0);
}

int changedir(char *const *args){
    // Implementation of cd
    path[0] = '\0';

    // If nothing is given go to the home directory
    if (args[1] == '\0'){
        // Create a path from the "/home/" string and the username
        strcat(path, "/home/");
        strcat(path, whoami());
    } else {
        // If a path IS given copy the string to the path variable
        strcpy(path, args[1]);
    }

    // Change the dir + errors
    if (!chdir(path))
        memcpy(g_curpath, path, strlen(path) + 1);
    else
        printf("cd: no such directory: %s\n", path);

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
        // Output errors
        int err = execvp(args[0], args);
        if (err == -1){
            printf("kosmos: command not found: %s\n", *args);
            exit(-1);
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
                printf("kosmos: too many arguments\n");
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

const char *prompt(const char *homepath){
    // Create a prompt for the user
    // This changes the homepath to ~ like other shells
    char cwd[PATHLEN];
    char *ret;

    getcwd(cwd, PATHLEN);

    char *path = malloc(sizeof(cwd)+sizeof(homepath)+1);

    // If cwd contains the homepath replace the homepath with ~
    ret = strstr(cwd, homepath);
    if (ret){
        path[0] = '~';
        memmove(cwd, cwd+strlen(homepath), sizeof(cwd));
        // Add the hyphen
        strcat(path, cwd);
        // Push all that into cwd
        strcpy(cwd, path);
    }
    free(path);
    snprintf(shell_prompt, sizeof(shell_prompt), "[%s%s%s@%s%s%s]:%s%s%s$ ",
            YELLOWBOLD, whoami(), RESET,
            BLUE, get_host(), RESET,
            MAGENTA, cwd, RESET);
    return shell_prompt;
}

void mainloop(){
    char **args = NULL;
    const char *homepath = get_homepath();
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

void init(){
    getcwd(g_curpath, PATHLEN);
}

int main(int arg_counter,char* argv[]){
    command_line_arguments(arg_counter, argv);
    init();
    mainloop();
    return 0;
}
