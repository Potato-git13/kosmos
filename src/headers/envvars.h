#ifndef ENVVARS_HEADER
#define ENVVARS_HEADER

void env_vars(char **args, int argc);

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

        // Remove everything after space/NULL
        char *after = strchr(p, ' ');
        if (after != NULL) {
            // If length is 1 it can only be a space so add it back
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

#endif