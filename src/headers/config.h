#ifndef CONFIG_HEADER
#define CONFIG_HEADER

void read_config(char *filename);
void create_config(char *fname);
char *create_config_name(char *config_file);

void execute_command(char *const *args);
char **split_command(char *cmd, int *count);
void env_vars(char **args, int argc);
void comments(char *args);

void read_config(char *filename){
    FILE *fp = fopen(filename, "r");
    char buffer[MAX_LINES];
    char **args;
    int argc;

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
}

void create_config(char *fname){
    // If the file does not exist create it
    if (access(fname, F_OK) != 0) {
        FILE *fp;

        printf("kosmos: the config file was not found. Creating an empty one\n");
        fp = fopen(fname, "w");
        fclose(fp);
    }
}

char *create_config_name(char *config_file){
    // Create the config file path /home/USER/.kosmosrc
    strcpy(config_file, "/home/");
    strcat(config_file, whoami());
    strcat(config_file, "/.kosmosrc");

    return config_file;
}

#endif