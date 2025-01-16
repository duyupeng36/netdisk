#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#undef __USE_POSIX
#define __USE_POSIX
#include <string.h>

#include "cwd.h"

int cwd_create_path_node(path_node_t **node, const char *path) {
    *node = (path_node_t *)malloc(sizeof(path_node_t));
    if(*node == NULL) {
        return -1;
    }
    if (strlen(path) > PATH_NODE_MAX - 1)
    {
        free(*node);
        *node = NULL;
        return -1;
    }
    strncpy((*node)->path, path, strlen(path));
    (*node)->path[strlen(path)] = '\0';

    (*node)->next = NULL;
    (*node)->prev = NULL;
    return 0;
}
int cwd_create(cwd_t **cwd)
{
    *cwd = (cwd_t *)malloc(sizeof(cwd_t));
    if (*cwd == NULL)
    {
        return -1;
    }
    (*cwd)->top = NULL;
    (*cwd)->bottom = NULL;
    return 0;
}

int cwd_clear(cwd_t *cwd)
{
    path_node_t *node = cwd->top;
    while (node != NULL)
    {
        path_node_t *next = node->next;
        free(node);
        node = next;
    }
    cwd->top = NULL;
    cwd->bottom = NULL;
    return 0;
}

int cwd_push(cwd_t *cwd, char *path)
{
    if(*path == '/') {
        cwd_clear(cwd);
        path++;
    }

    char *path_node;
    while ((path_node = strtok_r(path, "/", &path)))
    {
        size_t len = strlen(path_node);
        if(strncmp(path_node, ".", len) == 0) {
            continue;
        }
        if(strncmp(path_node, "..", len) == 0) {
            cwd_pop(cwd);
            continue;
        }
        
        if(len > PATH_NODE_MAX -1) {
            return -1;
        }
        path_node_t *node = NULL;
        if(cwd_create_path_node(&node, path_node) != 0) {
            return -1;
        }

        // 插入栈顶
        if(cwd->top == NULL || cwd->bottom == NULL) {
            cwd->top = node;
            cwd->bottom = node;
        } else {
            node->next = cwd->top;
            cwd->top->prev = node;
            cwd->top = node;
        }
    }
    return 0;
}


int cwd_pop(cwd_t *cwd)
{
    if (cwd->top != NULL)
    {
        cwd->top = cwd->top->next;
        if(cwd->top != NULL) {
            cwd->top->prev = NULL;
        } else {
            cwd->bottom = NULL;
        }
        return 0;
    }
    return -1;
}

int cwd_pwd(cwd_t *cwd, char *buf, size_t size)
{
    // 从栈底 cwd->bottom 到栈顶 cwd->top 拼接路径
    path_node_t *node = cwd->bottom;
    if(node == NULL) {
        buf[0] = '/';
        buf[1] = '\0';
        return 0;
    }
    size_t len = 0;
    while (node != NULL)
    {
        size_t node_len = strlen(node->path);
        if(len + node_len + 1 > size) {
            return -1;
        }
        if(len >= 0 && len < size-1) {
            buf[len++] = '/';
        }
        strncpy(buf + len, node->path, node_len);
        len += node_len;
        node = node->prev;
        printf("node: %p\n", node);
    }
    buf[len] = '\0';
    return 0;
}
