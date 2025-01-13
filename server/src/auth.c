#include <stdlib.h>

#include "auth.h"

static user_t *user_create(int fd) {
    user_t *user = malloc(sizeof(user_t));
    if (user == NULL) {
        return NULL;
    }
    user->fd = fd;
    user->username = NULL;
    user->cwd = cwd_create(256);
    if (user->cwd == NULL) {
        free(user);
        return NULL;
    }
    user->status = USER_UNLOGIN;
    user->next = NULL;
    return user;
}

user_table_t *user_table_init(int capacity) {
    user_table_t *table = malloc(sizeof(user_table_t));
    if (table == NULL) {
        return NULL;
    }
    table->users = malloc(sizeof(user_t *) * capacity);
    if (table->users == NULL) {
        free(table);
        return NULL;
    }
    for (int i = 0; i < capacity; i++)
    {
        table->users[i] = NULL;
    }
    table->size = 0;
    table->capacity = capacity;
    return table;
}


static int hash(int fd, int capacity) {
    return fd % capacity;
}

user_t *user_table_get(user_table_t *table, int fd) {
    int index = hash(fd, table->capacity);
    user_t *current = table->users[index];
    while (current)
    {
        if(current->fd == fd) {
            return current;
        }
        current = current->next;
    }
    return NULL;   
}

int user_table_add(user_table_t *table, int fd) {
    if(table->size == table->capacity) {
        return -1;
    }
    int index = hash(fd, table->capacity);
    if(table->users[index] == NULL) {
        table->users[index] = user_create(fd);
        if(table->users[index] == NULL) {
            return -1;
        }
    } else {
        user_t *current = table->users[index];
        while (current->next)
        {
            current = current->next;
        }
        current->next = user_create(fd);
        if(current->next == NULL) {
            return -1;
        }
    }
    table->size++;
    return 0;
}
