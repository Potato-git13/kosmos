#ifndef PROMPT_HEADER
#define PROMPT_HEADER

const char *prompt(const char *homepath);
void str_replace(char *stack, const char *needle, const char *replacement);

char shell_prompt[1024];

void str_replace(char *stack, const char *needle, const char *replacement)
{
    char buffer[1024] = {0};
    char *insert_point = &buffer[0];
    const char *tmp = stack;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        // Walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // Copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // Copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        tmp = p + needle_len;
    }

    // Write the finished string back to stack
    strcpy(stack, buffer);
}

const char *prompt(const char *homepath){
    // Create a prompt for the user
    char cwd[PATHLEN];
    char *cwd_check;
    char *PS1 = getenv("PS1");
    if (!PS1){
        fprintf(stderr, "kosmos: prompt not found, using default one\n");
        strcpy(shell_prompt, "$ ");
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
    char *host = get_host();

    // User
    str_replace(shell_prompt, "\\u", whoami());
    // Host
    str_replace(shell_prompt, "\\h", host);
    // cwd
    str_replace(shell_prompt, "\\s", cwd);

    char *color_arr[] = {RED, REDBOLD, GREEN, GREENBOLD,
                        YELLOW, YELLOWBOLD, BLUE, BLUEBOLD,
                        MAGENTA, MAGENTABOLD, CYAN, CYANBOLD,
                        RESET};
    char *color_symbols[] = {"\\r\\", "\\rb\\", "\\g\\", "\\gb\\",
                        "\\y\\", "\\yb\\", "\\b\\", "\\bb\\",
                        "\\m\\", "\\mb\\", "\\c\\", "\\cb\\",
                        "\\reset\\"}; 
    
    // Asign a symbol for every color
    for (int i = 0; i < SIZE(color_arr); i++){
        str_replace(shell_prompt, color_symbols[i], color_arr[i]);
    }

    free(host);
    return shell_prompt;
}

#endif
