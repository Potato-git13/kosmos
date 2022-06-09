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
#include "headers/builtins.h"
#include "headers/history.h"

#include "config.h"

extern int errno;

char *g_args[COMMANDLEN];
char dest[COMMANDLEN];

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
    if (str[len] == '\n')
        len--;
    char *new_str = malloc(len * sizeof(char *));
    memcpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

char **split_command(char *cmd, int *count){
    char *lastptr = cmd;
    int   argcnt = 0;
    int   len = 0;

    /*
        Split the string given for every space and NULL character and return
        the array of arguments when finished
    */
    while (1){
        if (*cmd == ' ' || *cmd == '\0'){
            // Create a new string and add it to the array
            g_args[argcnt++] = create_string(lastptr, len);

            /*
                If a NULL character is encountered that means the end of the
                command therefore return the array of arguments and exit the
                function
            */
            if (*cmd == '\0'){
                g_args[argcnt] = 0;
                *count = argcnt;
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

void mainloop(){
    char **args = NULL;
    int argc;
    const char *homepath = get_homepath();

    char hist_file[sizeof(homepath) + sizeof("/.kosmos_history") * sizeof(char *)];
    strcpy(hist_file, homepath);
    strcat(hist_file, "/.kosmos_history");    

    set_history(hist_file);

    while (1){
        char *g_buffer = malloc(COMMANDLEN);
        g_buffer = readline(prompt(homepath));
        // Handle history
        // Create the history file if it doesn't exist
        create_history(hist_file);
        // If the buffer is not NULL save it to history
        if (*g_buffer != '\0'){
            add_history(g_buffer);
            append_history(1, hist_file);
        }
        history_truncate_file(hist_file, SAVEHIST);
        // Trim the buffer
        trim(g_buffer, g_buffer);
        // Aliases
        for (int i = 0; i < (int)SIZE(aliases); i++){
            alias(dest, g_buffer, aliases[i].substring, aliases[i].replace);
        }
        // Set dest to g_buffer if dest is NULL
        if (!*dest){
            strcpy(dest, g_buffer);
        }
        // Split the command
        args = split_command(dest, &argc);
        // Reset dest and free g_buffer
        memset(dest, 0, strlen(dest));
        free(g_buffer);
        // Execute the given command
        execute_command(args);
        for (int i = 0; i < argc; i++)
            free(args[i]);
    }
}

int main(int argc,char* argv[]){
    // Check the arguments
    command_line_arguments(argc, argv);
    // Enter the main loop of the shell
    mainloop();
    return 0;
}
