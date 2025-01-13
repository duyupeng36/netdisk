#ifndef SERVICE_H
#define SERVICE_H

#include "task.h"

/**
 * @brief receive command from client
 * 
 * @param fd client socket fd
 * @param args pointer to the command arguments
 * @param argc pointer to the number of arguments
 * @return int 0 on success, -1 on error
 */
int recv_cmd(int fd, char ***args, int *argc);

/**
 * @brief get the handler of the command
 * 
 * @param cmd the command, which is the first argument of the command
 * @return task_handler_t the handler of the command
 */
task_handler_t cmd_handler(const char *cmd);


/**
 * @brief The　following　functions　are　the　handlers　of　the　commands
 * 
 * @param fd the file descriptor of the network communication
 * @param argc the number of arguments
 * @param argv the arguments. argv[0] is the command, and the others are the arguments
 * @return int  0 on success, -1 on error
 */
int handle_login(user_t *user, int argc, char *argv[]);

int handle_ls(user_t *user, int argc, char *argv[]);

int handle_cd(user_t *user, int argc, char *argv[]);

int handle_pwd(user_t *user, int argc, char *argv[]);

int handle_gets(user_t *user, int argc, char *argv[]);

int handle_puts(user_t *user, int argc, char *argv[]);

int handle_rm(user_t *user, int argc, char *argv[]);

int handle_mkdir(user_t *user, int argc, char *argv[]);

int handle_rmdir(user_t *user, int argc, char *argv[]);

int handle_quit(user_t *user, int argc, char *argv[]);

#endif // SERVICE_H
