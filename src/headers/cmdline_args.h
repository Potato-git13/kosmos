#ifndef CMDLINEARGS_HEADER
#define CMDLINEARGS_HEADER

void command_line_arguments(int argc, char *argv[]);

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
        // Open the the first argument and execute line by line
        char *filename = argv[1];
        FILE *fp = fopen(filename, "r");
        char buffer[MAX_LINES];
        char **args;
        int argc;

        if (fp == NULL){
            printf("kosmos: can't open file: %s\n", filename);
            exit(EXIT_FAILURE);
        }

        while(fgets(buffer, sizeof(buffer), fp)){
            // Handle comments
            comments(buffer);
            // Trim the buffer
            trim(buffer, buffer);

            // Split the arguments and execute them
            args = split_command(buffer, &argc);
            env_vars(args, argc);
            execute_command(args);

            // Reset the buffer and free the arguments
            memset(buffer, 0, strlen(buffer));
            for (int i = 0; i < argc; i++)
                free(args[i]);
        }

        exit(EXIT_SUCCESS);
    }
}

#endif