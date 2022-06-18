#ifndef PROMPT_HEADER
#define PROMPT_HEADER

const char *prompt(const char *homepath);

char shell_prompt[512];

const char *prompt(const char *homepath){
    // Create a prompt for the user
    char cwd[PATHLEN];
    char *cwd_check;

    cwd_check = getcwd(cwd, PATHLEN);
    if (!cwd_check){
        return NULL;
    }

    char *path = malloc(PATHLEN);
    // This changes the homepath to ~ like other shells
    // If cwd contains the homepath replace the homepath with ~
    if (strstr(cwd, homepath)){
        strcpy(path, "~");
        memmove(cwd, cwd + strlen(homepath), sizeof(cwd));
        // Add the hyphen
        strcat(path, cwd);
        // Push all that into cwd
        strcpy(cwd, path);
    }
    free(path);
    char *host = get_host();
    // Combine everything into a string and return it
    snprintf(shell_prompt, sizeof(shell_prompt), "[%s%s%s@%s%s%s]:%s%s%s$ ",
            YELLOWBOLD, whoami(), RESET,
            BLUE, host, RESET,
            MAGENTA, cwd, RESET);
    free(host);
    return shell_prompt;
}

#endif
