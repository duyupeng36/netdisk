#ifndef COMMON_H

#include "cwd.h"

typedef struct online_user online_user_t;

struct online_user {
    int fd;
    char *username;
    cwd_t *cwd;
};

/**
 * @brief task handler function
 * 
 * @param fd the file descriptor of the network communication
 * @param argc the number of arguments
 * @param argv the arguments. argv[0] is the command, and the others are the arguments
 * @return on success, return 0; on error, return -1
 */
typedef int(*task_handler_t)(int fd, int argc, char *argv[]);

#endif // COMMON_H