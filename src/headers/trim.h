#ifndef TRIM_HEADER
#define TRIM_HEADER

void trim (char *dest, char *src)
{
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

#endif
