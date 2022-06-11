#ifndef CONFIG_HEADER
#define CONFIG_HEADER

void execute_command(char *const *args);
char **split_command(char *cmd, int *count);

void read_config(char *filename){
    FILE *fp = fopen(filename, "r");
    char buffer[MAX_LINES];
    char **args;
    int argc;

    while(fgets(buffer, sizeof(buffer), fp)){
        // Trim the buffer
        trim(buffer, buffer);

        // Split the arguments and execute them
        args = split_command(buffer, &argc);
        execute_command(args);

        // Reset the buffer and free the arguments
        memset(buffer, 0, strlen(buffer));
        for (int i = 0; i < argc; i++)
            free(args[i]);
    }
}

#endif