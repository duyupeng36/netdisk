#include <stdlib.h>
#include <stdio.h>

#include "connection.h"
#include "cwd.h"

int connction_create(connection_t **conn, int fd) {
    *conn = (connection_t *)malloc(sizeof(connection_t));
    if (*conn == NULL) {
        return -1;
    }
    (*conn)->fd = fd;
    // 初始化当前工作目录
    if(cwd_create(&(*conn)->cwd) != 0) {
        free(*conn);
        return -1;
    }
    (*conn)->next = NULL;
    return 0;
}

static int hash(int fd, int capacity) {
    return fd % capacity;
}

int connect_table_create(connect_table_t **table, int capacity) {
    *table = (connect_table_t *)malloc(sizeof(connect_table_t));
    if (*table == NULL) {
        return -1;
    }
    (*table)->data = (connection_t **)malloc(sizeof(connection_t *) * capacity);
    if ((*table)->data == NULL) {
        free(*table);
        return -1;
    }
    for (int i = 0; i < capacity; i++) {
        (*table)->data[i] = NULL;
    }
    (*table)->capacity = capacity;
    return 0;
}

int connect_table_insert(connect_table_t *table, connection_t *conn) {
    int index = hash(conn->fd, table->capacity);
    conn->next = table->data[index];
    table->data[index] = conn;
    return 0;
}

int connect_table_remove(connect_table_t *table, int fd) {
    int index = hash(fd, table->capacity);
    connection_t *prev = NULL;
    connection_t *conn = table->data[index];
    while (conn != NULL) {
        if (conn->fd == fd) {
            if (prev == NULL) {
                table->data[index] = conn->next;
            } else {
                prev->next = conn->next;
            }
            free(conn);
            return 0;
        }
        prev = conn;
        conn = conn->next;
    }
    return -1;
}

int connect_table_find(connect_table_t *table, int fd, connection_t **conn) {
    int index = hash(fd, table->capacity);
    connection_t *c = table->data[index];
    while (c != NULL) {
        if (c->fd == fd) {
            *conn = c;
            return 0;
        }
        c = c->next;
    }
    return -1;
}