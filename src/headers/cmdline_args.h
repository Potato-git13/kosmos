#ifndef CMDLINEARGS_HEADER
#define CMDLINEARGS_HEADER

void command_line_arguments(int argc, char *argv[]);

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
        printf("\
Usage: kosmos [<option>]\n\n\
Options:\n\
\t-h, --help\tshow this message, then exit\n\
\t-v, --version\tshow the kosmos version number, then exit\n\
\t-c\t\tread the commands from the arguments after the flag, then exit\n");
        exit(EXIT_SUCCESS);
    // Version number
    } else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")){
        printf("kosmos %s\n", VERSION);
        exit(EXIT_SUCCESS);
    }
}

#endif