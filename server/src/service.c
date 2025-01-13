#include <sys/uio.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shadow.h>

#define _DEFAULT_SOURCE  /* Since glibc 2.28*/
#include <crypt.h>

#include "service.h"

int recv_cmd(int fd, char ***args, int *argc)
{
    // 读取 argc
    if (recv(fd, argc, sizeof(int), 0) != sizeof(int))
    {
        return -1;
    }
    // 读取每个参数的长度
    int argLenVec[*argc];
    if (recv(fd, argLenVec, sizeof(int) * (*argc), 0) != sizeof(int) * (*argc))
    {
        return -1;
    }
    int totalLen = 0;
    for (int i = 0; i < *argc; i++)
    {
        totalLen += argLenVec[i];
    }

    // printf("totalLen: %d\n", totalLen);

    // 为每个参数分配空间
    *args = malloc(sizeof(char *) * (*argc + 1));
    (*args)[*argc] = NULL;
    for (int i = 0; i < *argc; i++)
    {
        (*args)[i] = malloc(argLenVec[i] + 1);
    }

    // 读取每个参数
    struct iovec iov[*argc];
    for (int i = 0; i < *argc; i++)
    {
        iov[i].iov_base = (*args)[i];
        iov[i].iov_len = argLenVec[i];
    }

    if (recvmsg(fd, &(struct msghdr){.msg_iov = iov, .msg_iovlen = *argc}, 0) != totalLen)
    {
        return -1;
    }

    for (int i = 0; i < *argc; i++)
    {
        (*args)[i][argLenVec[i]] = '\0';
    }
    return 0;
}

task_handler_t cmd_handler(const char *cmd)
{
    if (strcmp(cmd, "login") == 0)
    {
        return handle_login;
    }
    else if (strcmp(cmd, "ls") == 0)
    {
        return handle_ls;
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        return handle_cd;
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        return handle_pwd;
    }
    else if (strcmp(cmd, "gets") == 0)
    {
        return handle_gets;
    }
    else if (strcmp(cmd, "puts") == 0)
    {
        return handle_puts;
    }
    else if (strcmp(cmd, "rm") == 0)
    {
        return handle_rm;
    }
    else if (strcmp(cmd, "mkdir") == 0)
    {
        return handle_mkdir;
    }
    else if (strcmp(cmd, "rmdir") == 0)
    {
        return handle_rmdir;
    }
    else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0)
    {
        return handle_quit;
    }
    else
    {
        return NULL;
    }
}

int handle_login(user_t *user, int argc, char *argv[]) {
    // TODO: 实现 login
    printf("login\n");
    user->status = USER_LOINGED;
    user->username = "dyp";
    return 0;
}

int handle_ls(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 ls
    printf("ls\n");
    printf("username: %s\n", user->username);
    return 0;
}

int handle_cd(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 cd
    printf("cd\n");
    return 0;
}

int handle_pwd(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 pwd
    printf("pwd\n");
    return 0;
}

int handle_gets(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 gets
    printf("gets\n");
    return 0;
}

int handle_puts(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 puts
    printf("puts\n");
    return 0;
}

int handle_rm(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 rm
    printf("rm\n");
    return 0;
}

int handle_mkdir(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 mkdir
    printf("mkdir\n");
    return 0;
}

int handle_rmdir(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 rmdir
    printf("rmdir\n");
    return 0;
}

int handle_quit(user_t *user, int argc, char *argv[])
{
    // TODO: 实现 quit
    printf("quit\n");
    return 0;
}
