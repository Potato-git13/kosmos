#ifndef PROMPT_HEADER
#define PROMPT_HEADER

void str_replace(char *stack, const char *needle, const char *replacement);
const char *prompt(const char *homepath);

char shell_prompt[1024];
int prompt_err_shown = 0;

const char *prompt(const char *homepath){
    // Create a prompt for the user
    char cwd[PATHLEN];
    char *cwd_check;
    // Read the PS1 environment variable for the prompt
    char *PS1 = getenv("PS1");
    if (!PS1){
        if (prompt_err_shown == 0){
            fprintf(stderr, "kosmos: prompt not found, using default one\n");
            prompt_err_shown++;
        }
        strcpy(shell_prompt, "\\u:\\s$ ");
    } else {
        strcpy(shell_prompt, PS1);
    }

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
    // Get the host name
    char *host = get_host();

    // User
    str_replace(shell_prompt, "\\u\\", whoami());
    // Host
    str_replace(shell_prompt, "\\h\\", host);
    // cwd
    str_replace(shell_prompt, "\\s\\", cwd);

    char *color_arr[] = {RED, REDBOLD, GREEN, GREENBOLD,
                        YELLOW, YELLOWBOLD, BLUE, BLUEBOLD,
                        MAGENTA, MAGENTABOLD, CYAN, CYANBOLD,
                        RESET};
    char *color_symbols[] = {"\\r\\", "\\rb\\", "\\g\\", "\\gb\\",
                        "\\y\\", "\\yb\\", "\\b\\", "\\bb\\",
                        "\\m\\", "\\mb\\", "\\c\\", "\\cb\\",
                        "\\reset\\"}; 
    
    // Asign a symbol for every color
    for (int i = 0; i < (int)SIZE(color_arr); i++){
        str_replace(shell_prompt, color_symbols[i], color_arr[i]);
    }

    free(host);
    return shell_prompt;
}

#endif
