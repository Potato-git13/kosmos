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
#include "headers/alias.h"
#include "config.h"

extern int errno;

char  *g_buffer;
char  *g_args[MAXARGS];
char   g_path[PATHLEN];

void command_line_arguments(int argc, char *argv[]){
    // If only one argument is given i.e "kosmos" drop to shell
    if (argc == 1){
        return;
    }

    // If there are to many arguments print an error and exits
    if (argc > 2){
        fprintf(stderr, "kosmos: too many arguments supplied\n");
        exit(EXIT_FAILURE);
    // Help message
    } else if (!strcmp(argv[1], "-h")){
        printf("\
Usage: kosmos [<option>]\n\n\
Options:\n\
\t-h\tshow this message, then exit\n\
\t-v\tshow the kosmos version number, then exit\n");
        exit(EXIT_SUCCESS);
    // Version number
    } else if (!strcmp(argv[1], "-v")){
        printf("kosmos %s\n", VERSION);
        exit(EXIT_SUCCESS);
    }
}

int builtin_exit(){
    exit(EXIT_SUCCESS);
}

int changedir(char *const *args){
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

    return 0;
}

// Define the builtins struct
typedef int (*builtin_fpointer)(char *const *args);
struct builtins{
    char *           name;
    builtin_fpointer func;
};

// Define the contents of the builtins struct
const struct builtins builtins[] = {
    {"exit", &builtin_exit},
    {"cd", &changedir}};

int builtin(char *const *args){
    // If the arguments given are the same as some builtin commands' name call them
    for (int i = 0; i < (int)SIZE(builtins); i++){
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
        printf("kosmos: %s\n", strerror(errno));
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

    /*
        Split the string given for every space and NULL character and return
        the array of arguments when finished
    */
    while (1){
        if (*cmd == ' ' || *cmd == '\0'){
            if (argcnt == MAXARGS){
                fprintf(stderr, "kosmos: too many arguments\n");
                return NULL;
            }
            // Create a new string and add it to the array
            g_args[argcnt++] = create_string(lastptr, len);

            /*
                If a NULL character is encountered that means the end of the
                command therefore return the array of arguments and exit the
                function
            */
            if (*cmd == '\0'){
                g_args[argcnt] = 0;
                return g_args;
            }

            len = 0;
            lastptr = ++cmd;
            continue;
        }
        cmd++;
        len++;
    }

    return NULL;
}

void create_history(char *fname){
    if (access(fname, F_OK) != 0) {
        FILE *fp;
        fp = fopen(fname, "w");
        fclose(fp);
        create_history(fname);
    }
}

void mainloop(){
    char **args = NULL;
    const char *homepath = get_homepath();

    char *hist_file = malloc(PATHLEN);
    strcpy(hist_file, homepath);
    strcat(hist_file, "/.kosmos_history");

    // TAB autocomplete
    rl_bind_key('\t', rl_complete);
    // Limit the history size
    stifle_history(HISTSIZE);
    // Create a history file if it cannot be read
    if(read_history(hist_file)!=0)
        create_history(hist_file);

    while (1){
        char *dest = malloc(COMMANDLEN * sizeof(char));
        g_buffer = readline(prompt(homepath));
        // Handle history
        // If the buffer is not NULL save it to history
        if (*g_buffer != '\0'){
            add_history(g_buffer);
            append_history(1, hist_file);
        }
        history_truncate_file(hist_file, SAVEHIST);
        // Aliases
        for (int i = 0; i < (int)SIZE(aliases); i++){
            alias(dest, g_buffer, aliases[i].substring, aliases[i].replace);
        }
        // If dest contains a string use it to be trimmed, else use g_buffer
        if(*dest)
            trim(g_buffer, dest);
        else
            trim(g_buffer, g_buffer);
        free(dest);
        // Split the command
        args = split_command(g_buffer);
        free(g_buffer);
        // Execute the given command
        execute_command(args);
        // Free the args array
        for(int i = 0; i < (int)sizeof(args); i++){
            free(args[i]);
        }
    }
}

int main(int argc,char* argv[]){
    // Check the arguments
    command_line_arguments(argc, argv);
    // Enter the main loop of the shell
    mainloop();
    return 0;
}
