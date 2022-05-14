#ifndef TRIM_HEADER
#define TRIM_HEADER

void trim (char *dest, char *src)
{
    // Check for exsitance
    if (!src || !dest)
       return;

    int len = strlen(src);
    if (!len) {
        *dest = '\0';
        return;
    }
    char *ptr = src + len - 1;
    char *q;

    // remove trailing whitespace
    while (ptr > src) {
        if (!isspace(*ptr))
            break;
        ptr--;
    }

    ptr++;

    // remove leading whitespace
    for (q = src; (q < ptr && isspace(*q)); q++);

    // Copy the results into dest
    while (q < ptr)
        *dest++ = *q++;

    *dest = '\0';
}

#endif
