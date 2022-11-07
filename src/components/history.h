#ifndef HISTORY_HEADER
#define HISTORY_HEADER

#include "util/util.h"
#include <readline/history.h>
#include <readline/readline.h>

void create_history(char *fname);
void set_history(char *hist_file);
void create_hist_name(const char *homepath, char *hist_file);

#endif