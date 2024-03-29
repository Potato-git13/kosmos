#ifndef CONFIG_HEADER
#define CONFIG_HEADER

void config_handler(const char *homepath);
void read_config(char *filename);
void create_config(char *filename);

void execute_command(char *const *args);
char **split_command(char *cmd, int *count);
void comments(char *args);

void config_handler(const char *homepath){
    char *config_file;
    // Create the config filename
    asprintf(&config_file, "%s/.kosmosrc", homepath);
    // If the config file doesn't exist create an empty one
    create_config(config_file);
    // Read the config file
    read_config(config_file);
    free(config_file);
}

char dest[COMMANDLEN];
const char *homepath;

void read_config(char *filename){
    homepath = get_homepath();
    FILE *fp = fopen(filename, "r");
    char buffer[MAX_LINES];
    char **args;
    int argc;

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
        args = split_command(dest, &argc);
        env_vars(args, argc);
        execute_command(args);

        // Reset the buffer and dest
        memset(buffer, 0, strlen(buffer));
        memset(dest, 0, strlen(dest));
        // Free the arguments
        for (int i = 0; i < argc; i++)
            free(args[i]);
    }
}

void create_config(char *filename){
    // If the file does not exist create it
    if (access(filename, F_OK) != 0) {
        FILE *fp;

        fprintf(stderr, "kosmos: the config file was not found. Creating an empty one\n");
        fp = fopen(filename, "w");
        fclose(fp);
    }
}

#endif