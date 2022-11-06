#ifndef HISTORY_HEADER
#define HISTORY_HEADER

void create_history(char *fname);
void set_history(char *hist_file);
void create_hist_name(const char *homepath, char *hist_file);

void create_history(char *fname){
    // If the file does not exist create it
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
    if(read_history(hist_file) != 0)
        create_history(hist_file);
}

void create_hist_name(const char *homepath, char *hist_file){
    // Concatinating the homepath and "/.kosmos_history"
    strcpy(hist_file, homepath);
    strcat(hist_file, "/.kosmos_history"); 
}

#endif