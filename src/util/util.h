#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "components/defvars.h"

void alias(char *dest, const char *src, const char* substring, const char *replace);
void trim(char *dest, char *src);
void str_replace(char *stack, const char *needle, const char *replacement);
const char* whoami();
char* get_host();
char* get_homepath();
void env_vars(char **args, int argc);

#endif