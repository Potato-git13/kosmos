#ifndef PROMPT_HEADER
#define PROMPT_HEADER

#include "util/util.h"
#include "util/colors.h"

void str_replace(char *stack, const char *needle, const char *replacement);
const char *prompt(const char *homepath);

#endif
