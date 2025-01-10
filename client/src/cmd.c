#include <ctype.h>
#include <stdlib.h>
#include <string.h>


#include <cmd.h>


int cmd_read(char * restrict buf, size_t size, const char * restrict prompt) {
    printf("%s", prompt);
    size_t len = 0;
    while (len < size - 1 && (buf[len] = getchar()) != '\n') {
        len++;
    }
    buf[len] = '\0';
    return len > 0 ? len : -1;
}

char ** cmd_parse(const char *cmd) {

    // 跳过前导空白字符串
    while(isspace(*cmd)) {
        cmd++;
    }
    // 计算参数个数
    int argc = 0;

    int i = 0;
    while (cmd[i] != '\0')
    {
        // 跳过非空白字符
        while(!isspace(cmd[i]) && cmd[i] != '\0') {
            i++;
        }
        // 参数个数 +1
        argc++;
        // 跳过当前空白字符
        while(isspace(cmd[i]) && cmd[i] != '\0') {
            i++;
        }
    }
    // printf("argc = %d\n", argc);
    // 分配参数数组
    char **argv = (char **)malloc((argc + 1) * sizeof(char *));
    if(argv == NULL) {
        return NULL;
    }
    
    // 解析参数
    argc = 0;
    const char *star = cmd;
    while (*cmd)
    {
        // 跳过前导非空白字符
        while (!isspace(*cmd) && *cmd != '\0') {
            cmd++;
        }
        // 分配存储参数的内存空间
        argv[argc] = (char *)malloc((cmd - star + 1) * sizeof(char));
        // 分配失败
        if(argv[argc] == NULL) {
            // 释放之前分配的内存
            for(int j = 0; j < argc; j++) {
                free(argv[j]);
            }
            free(argv);
            return NULL;
        }
        // 复制参数
        strncpy(argv[argc], star, cmd - star);
        argv[argc][cmd - star] = '\0';
        // 参数个数 +1
        argc++;
        // 跳过当前空白字符
        while(isspace(*cmd) && *cmd != '\0') {
            cmd++;
        }
        // 记录下一个参数的起始位置
        star = cmd;
    }
    argv[argc] = NULL;
    return argv;
}


void cmd_exec(char **args) {
    for(char **p = args; *p != NULL; p++) {
        printf("%s\n", *p);
    }
}


void cmd_free(char **args) {
    for(char **p = args; *p != NULL; p++) {
        free(*p);
    }
    free(args);
}