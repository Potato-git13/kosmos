#ifndef ALIAS_HEADER
#define ALIAS_HEADER

void alias(char *dest, const char *src, const char* substring, const char *replace);

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

#endif
