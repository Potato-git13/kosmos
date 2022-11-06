#ifndef STRREPLACE_HEADER
#define STRREPLACE_HEADER

void str_replace(char *stack, const char *needle, const char *replacement);

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

#endif