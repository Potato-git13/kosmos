#ifndef HISTORY_HEADER
#define HISTORY_HEADER

#include "../config.h"

void create_history(char *fname){
    if (access(fname, F_OK) != 0) {
        FILE *fp;
        fp = fopen(fname, "w");
        fclose(fp);
        create_history(fname);
    }
}

void set_history(char *hist_file){
    // TAB autocomplete
    rl_bind_key('\t', rl_complete);
    // Limit the history size
    stifle_history(HISTSIZE);
    // Create a history file if it cannot be read
    if(read_history(hist_file)!=0)
        create_history(hist_file);
}

#endif