#include <stdio.h>

#include "message.h"

#include "response.h"


int response_mkdir(int fd, struct state *state) {
    return 0;
}

int response_rmdir(int fd, struct state *state) {
    return 0;
}


int response_ls(int fd, struct state *state) {
    return 0;
}

int response_cd(int fd, struct state *state) {
    char *message = NULL;
    if(message_recv(fd, &message) == -1) {
        fprintf(stderr, "Failed to receive message\n");
        return -1;
    }
    printf("%s\n", message);
    return 0;
}



int response_pwd(int fd, struct state *state) {
    return 0;
}


int response_gets(int fd, struct state *state) {
    return 0;
}


int response_puts(int fd, struct state *state) {
    return 0;
}

int response_remove(int fd, struct state *state) {
    return 0;
}
