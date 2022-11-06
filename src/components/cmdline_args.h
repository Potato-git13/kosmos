#ifndef CMDLINEARGS_HEADER
#define CMDLINEARGS_HEADER

void command_line_arguments(int argc, char *argv[]);

char dest[COMMANDLEN];

const char help_msg[] =
    "kosmos, version "VERSION"\n"
    "Usage: kosmos [<option>]\n"
    "\n"
    "Options:\n"
    "\t-h, --help\tshow this message, then exit\n"
    "\t-v, --version\tshow the kosmos version number, then exit\n"
    "\t-c\t\tread the commands from the arguments after the flag, then exit\n"
    ;

void command_line_arguments(int argc, char *argv[]){
    // If only one argument is given i.e "kosmos" drop to shell
    if (argc == 1){
        return;
    }
    // Command is read from the arguments
    if (!strcmp(argv[1], "-c")){
        // Remove the first 2 arguments from the array i.e. "kosmos", "-c"
        if (!argv[2]){
            fprintf(stderr, "kosmos: no arguments to be executed\n");
            exit(EXIT_FAILURE);
        }
        for (int i=0; i<argc; i++){
            argv[i]=argv[i+2];
        }
        execute_command(argv);
        exit(EXIT_SUCCESS);
    }
    // If there are to many arguments print an error and exits
    if (argc > 2){
        fprintf(stderr, "kosmos: too many arguments supplied\n");
        exit(EXIT_FAILURE);
    // Help message
    } else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")){
        printf("%s", help_msg);
        exit(EXIT_SUCCESS);
    // Version number
    } else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")){
        printf("kosmos %s\n", VERSION);
        exit(EXIT_SUCCESS);
    } else {
        char *homepath = get_homepath();
        // Open the the first argument and execute line by line
        char *filename = argv[1];
        FILE *fp = fopen(filename, "r");
        char buffer[MAX_LINES];
        char **args;
        int argcnt;

        if (fp == NULL){
            printf("kosmos: can't open file: %s\n", filename);
            exit(EXIT_FAILURE);
        }

        while(fgets(buffer, sizeof(buffer), fp)){
            // Handle comments
            comments(buffer);
            // Trim the buffer
            trim(buffer, buffer);

            // Aliases
            for (int i = 0; i < nelements; i++){
                alias(dest, buffer, aliases[i]->substring, aliases[i]->replace);
            }
            // Set dest to buffer if dest is NULL
            if (!*dest){
                strcpy(dest, buffer);
            }

            // Replace a ~ with the homepath
            str_replace(dest, "~", homepath);

            // Split the arguments and execute them
            args = split_command(dest, &argcnt);
            env_vars(args, argcnt);
            execute_command(args);

            // Reset the buffer and free the arguments
            memset(buffer, 0, strlen(buffer));
            memset(dest, 0, strlen(dest));
            for (int i = 0; i < argcnt; i++)
                free(args[i]);
        }

        exit(EXIT_SUCCESS);
    }
}

#endif