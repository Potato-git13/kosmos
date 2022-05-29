#ifndef GET_HEADER
#define GET_HEADER

int gethostname(char *name, size_t len);

const char* whoami(){
    // Get the name of the user
    char *user=getenv("USER");
    if(user==NULL)
        printf("Error: user could not be found");

    return user;
}

const char* get_host(){
    // Get the name of the host machine
    char* host = malloc(HOST_NAME_MAX);
    gethostname(host, HOST_NAME_MAX);
    return host;
}

const char* get_homepath(){
    char *homepath = malloc(PATHLEN);
    // Concatinate the string "/home/" and the username
    strcat(homepath, "/home/");
    strcat(homepath, whoami());
    return homepath;
}

#endif
