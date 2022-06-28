#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "defvars.h"
#include "colors.h"
#include "trim.h"
#include "get.h"
#include "prompt.h"
#include "alias.h"
#include "builtins.h"
#include "history.h"
#include "config.h"
#include "cmdline_args.h"
#include "strreplace.h"

extern int errno;

void execute_command(char *const *args);
char *create_string(char *str, int len);
char **split_command(char *cmd, int *count);
void env_vars(char **args, int argc);
void mainloop();
int main(int argc, char* argv[]);

char *g_args[COMMANDLEN];
char dest[COMMANDLEN];

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
    // Create a string set it to str, set the last character to NULL and return it
    char *new_str = malloc(len * sizeof(char *));
    memcpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

char **split_command(char *cmd, int *count){
    char *lastptr = cmd;
    int argcnt = 0;
    int len = 0;

    int quote_cnt = 0;
    int blocksize = 0;

    /*
        Split the string given for every space and NULL character and return
        the array of arguments when finished
    */
    while (1){
        if (*cmd == '"'){
            quote_cnt++;
            // First quote
            if(quote_cnt == 1){
                // Set the block size use in for loops
                blocksize = strlen(cmd);

                // Remove the quote
                for (int i = len; i < blocksize; i++){
                    cmd[i] = cmd[i+1];
                }

                cmd++;
                len++;

                continue;
            // Last quote
            }else if(quote_cnt == 2){
                // Remove the quote
                for (int i = len - blocksize + 2; i < blocksize; i++){
                    cmd[i] = cmd[i+1];
                }

                cmd++;
                len++;
                // Reset quote_cnt
                quote_cnt = 0;

                continue;
            }
        // Qoute count isn't 0 but also isn't a quote
        }else if (quote_cnt != 0){
            cmd++;
            len++;
            continue;
        }

        if (*cmd == '$'){
            len += COMMANDLEN;
        }

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

void env_vars(char **args, int argc){
    for (int i = 0; i < argc; i++){
        char *tmp = args[i];
        char *envvar_buf;
        char *buffer = malloc(COMMANDLEN);

        // Find an instance of $
        const char *p = strstr(tmp, "$");
        // If none are found go back to the top
        if (p == NULL){
            free(buffer);
            continue;
        }

        // If one is found increment p 
        p++;

        // Remove everything after space/null
        char *after = strchr(p, ' ');
        if (after != NULL) {
            // If tlength is 1 it can only be a space so add it back
            if (strlen(after) == 1){
                after++;
            }
            *after = '\0';
        }
        // Remove everthing before $
        char *before = strchr(tmp, '$');
        if (before != NULL){
            *before = '\0';
        }
        
        // Get the environment variable
        envvar_buf = getenv(p);

        /* 
        Check for a backslash if one is found replace it with a missing $,
        else replace the expression after the $ with its environment value
        */
        if(tmp[strlen(tmp)-1] == '\\'){
            tmp[strlen(tmp)-1] = '$';
            strcpy(buffer, tmp);
            strcat(buffer, p);
        } else if (envvar_buf){
            strcpy(buffer, tmp);
            strcat(buffer, envvar_buf);
        }

        strcpy(args[i], buffer);
        free(buffer);
    }
}

void mainloop(){
    char **args = NULL;
    int argc;
    const char *homepath = get_homepath();

    // Create the history filename for a single use and is then redefined inside the loop
    char *hist_file = malloc(sizeof(homepath) + sizeof("/.kosmos_history") * sizeof(char *));
    create_hist_name(homepath, hist_file);

    // History setup
    set_history(hist_file);
    free(hist_file);

    // Main loop
    while (1){
        // Create the history filename
        char *hist_file = malloc(sizeof(homepath) + sizeof("/.kosmos_history") * sizeof(char *));
        create_hist_name(homepath, hist_file);

        char *g_buffer = malloc(COMMANDLEN);
        // Get user input
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
        for (int i = 0; i < nelements; i++){
            alias(dest, g_buffer, aliases[i]->substring, aliases[i]->replace);
        }
        // Set dest to g_buffer if dest is NULL
        if (!*dest){
            strcpy(dest, g_buffer);
        }

        // Split the command and get the amount of arguments
        args = split_command(dest, &argc);
        // Reset dest and free g_buffer
        memset(dest, 0, strlen(dest));
        free(g_buffer);

        env_vars(args, argc);

        // Execute the given command
        execute_command(args);

        // Free the arguments
        for (int i = 0; i < argc; i++)
            free(args[i]);
        free(hist_file);
    }
}

int main(int argc, char* argv[]){
    char *config_file = malloc(sizeof("/home/") + sizeof(whoami()) + sizeof("/.kosmosrc") * sizeof(char *));

    // Initialise the aliases struct
    init_aliases();

    // Create the config filename
    create_config_name(config_file);
    // If the config file doesn't exist create an empty one
    create_config(config_file);
    // Read the config file
    read_config(config_file);
    free(config_file);

    // Check the arguments
    command_line_arguments(argc, argv);
    // Enter the main loop of the shell
    mainloop();
    return 0;
}
