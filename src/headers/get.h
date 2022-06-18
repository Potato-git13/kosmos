#ifndef GET_HEADER
#define GET_HEADER

const char* whoami();
char* get_host();
char* get_homepath();

// Define the function to avoid warnings during compilation
int gethostname(char *name, size_t len);

const char* whoami(){
    // Get the name of the user
    char *user = getenv("USER");
    if(user == NULL)
        printf("kosmos: user could not be found");

    return user;
}

char* get_host(){
    // Get the name of the host machine
    char* host = malloc(HOST_NAME_MAX);
    gethostname(host, HOST_NAME_MAX);
    return host;
}

char* get_homepath(){
    char *homepath = malloc(PATHLEN);
    // Concatinate the string "/home/" and the username
    strcpy(homepath, "/home/");
    strcat(homepath, whoami());
    return homepath;
}

#endif
