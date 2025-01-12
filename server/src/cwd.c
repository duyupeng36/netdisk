#include <stdlib.h>
#include <string.h>

#include "cwd.h"


cwd_t *cwd_create(int size) {
    cwd_t *cwd = malloc(sizeof(cwd_t));
    if (cwd == NULL) {
        return NULL;
    }

    cwd->path = calloc(sizeof(char *), size);
    if (cwd->path == NULL) {
        free(cwd);
        return NULL;
    }

    cwd->top = -1;
    cwd->size = size;
    return cwd;
}

void cwd_destroy(cwd_t *cwd) {
    for(int i = 0; i <= cwd->top; i++) {
        free(cwd->path[i]);
    }
    free(cwd->path);
    free(cwd);
}

int cwd_push(cwd_t *cwd, const char *path) {
    if(cwd->top == cwd->size - 1) {
        return -1;
    }

    cwd->top++;
    cwd->path[cwd->top] = malloc(strlen(path) + 1);
    if (cwd->path[cwd->top] == NULL) {
        return -1;
    }
    strncpy(cwd->path[cwd->top], path, strlen(path) + 1);
    return 0;
}

int cwd_pop(cwd_t *cwd) {
    if(cwd->top == -1) {
        return -1;
    }

    free(cwd->path[cwd->top]);
    cwd->top--;
    return 0;
}
