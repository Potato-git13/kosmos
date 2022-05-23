void alias(char *dest, const char *src, const char* substring, const char *replace){
    int substring_len = strlen(substring);
    // Check if src starts with substring
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
    strcpy(dest, replace);
    strcat(dest, src+substring_len);
}
