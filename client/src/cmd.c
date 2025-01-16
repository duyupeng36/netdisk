#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <json-c/json.h>

#include "cmd.h"
#include "tcp.h"
#include "response.h"

int command_read(command_t *command)
{
    char command_buffer[2048] = {0};
    fgets(command_buffer, sizeof(command_buffer), stdin);
    command_buffer[strlen(command_buffer)] = '\0';
    command->command_length = strlen(command_buffer);
    command->command = malloc((command->command_length + 1) * sizeof(char));
    if (command->command == NULL)
    {
        return -1;
    }
    strncpy(command->command, command_buffer, command->command_length);
    command->command[command->command_length] = '\0';
    return 0;
}

static bool is_valid_command(const char *command)
{
    // 检查命令是否合法
    // 跳过前导空白字符
    while (isspace(*command))
    {
        command++;
    }
    const char *start = command;
    // 跳过命令名
    while (!isspace(*command) && *command != '\0')
    {
        command++;
    }
    if (command == start)
    {
        return false;
    }
    if (strncmp(start, "login", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "register", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "mkdir", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "rmdir", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "ls", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "cd", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "pwd", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "gets", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "puts", command - start) == 0)
    {
        return true;
    }
    if (strncmp(start, "remove", command - start) == 0)
    {
        return true;
    }
    return false;
}

void command_free(command_t *command)
{
    free(command->command);
}

int command_send(int fd, command_t *command)
{
    // 检查命令是否合法
    if (!is_valid_command(command->command))
    {
        fprintf(stderr, "Invalid command: %s\n", command->command);
        return -1;
    }
    // 发送命令长度 : int sendn(int fd, const void *buf, size_t n)
    if (sendn(fd, &command->command_length, sizeof(command->command_length)) != sizeof(command->command_length))
    {
        return -1;
    }
    // 发送命令字符串
    if (sendn(fd, command->command, command->command_length) != command->command_length)
    {
        return -1;
    }
    return 0;
}

int command_execute(int fd, command_t *command, struct state *state)
{
    // 发送命令
    if (command_send(fd, command) == -1)
    {
        return -1;
    }

    // 接收命令执行结果
    if (strncmp(command->command, "mkdir", 5) == 0)
    {
        return response_mkdir(fd, state);
    }
    if (strncmp(command->command, "rmdir", 5) == 0)
    {
        return response_rmdir(fd, state);
    }
    if (strncmp(command->command, "ls", 2) == 0)
    {
        return response_ls(fd, state);
    }
    if (strncmp(command->command, "cd", 2) == 0)
    {
        return response_cd(fd, state);
    }
    if (strncmp(command->command, "pwd", 3) == 0)
    {
        return response_pwd(fd, state);
    }

    if (strncmp(command->command, "gets", 4) == 0)
    {
        return response_gets(fd, state);
    }
    if (strncmp(command->command, "puts", 4) == 0)
    {
        return response_puts(fd, state);
    }

    if (strncmp(command->command, "remove", 6) == 0)
    {
        return response_remove(fd, state);
    }

    return -1;
}
