#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#include "components/defvars.h"

#include "util.h"

// Define the function to avoid warnings during compilation
int gethostname(char *name, size_t len);

void alias(char *dest, const char *src, const char* substring, const char *replace){
    int substring_len = strlen(substring);
    // Check if src starts with substring if so set the value of dest to NULL and exit the function
    for (int i = 0; substring[i] != '\0'; i++){
       if (src[i] != substring[i]) {
           dest = NULL;
           return;
       }
    }
    // Check if the char after the substring in src is not 'space' or null and if so exit the function
    if (src[substring_len] != '\x20' && src[substring_len] != '\0'){
        return;
    }
    // Combine everything
    strcpy(dest, replace);
    strcat(dest, src+substring_len);
}

void trim(char *dest, char *src){
    // Check for existance
    if (!src || !dest)
       return;

    // Check if the length of src is 0, if so make dest NULL
    int len = strlen(src);
    if (!len) {
        *dest = '\0';
        return;
    }
    char *ptr = src + len - 1;
    char *q;

    // Remove trailing whitespace
    while (ptr > src) {
        if (!isspace(*ptr))
            break;
        ptr--;
    }

    ptr++;

    // Remove leading whitespace
    for (q = src; (q < ptr && isspace(*q)); q++);

    // Copy the results into dest
    while (q < ptr)
        *dest++ = *q++;

    *dest = '\0';
}

void str_replace(char *stack, const char *needle, const char *replacement){
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

const char* whoami(){
    // Get the name of the user
    char *user = getenv("USER");
    if(user == NULL)
        printf("kosmos: user could not be found");

    return user;
}

char* get_host(){
    // Get the name of the host machine
    char* host = malloc(HOST_NAME_MAX);
    gethostname(host, HOST_NAME_MAX);
    return host;
}

char* get_homepath(){
    char *homepath = malloc(PATHLEN);
    // Concatinate the string "/home/" and the username
    strcpy(homepath, "/home/");
    strcat(homepath, whoami());
    return homepath;
}

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
