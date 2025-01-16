#ifndef CONNECT_TABLE_H
#define CONNECT_TABLE_H

#include "cwd.h"


typedef struct connection connection_t;

struct connection {
    int fd;
    char username[21];
    char token[128];
    cwd_t *cwd;    
    connection_t *next;
};

typedef struct {
    connection_t **data;  // 存储指向连接的指针
    int capacity;  // 哈希表容量
} connect_table_t;

/**
 * @brief 创建 connection
 * 
 * @param conn 存储connection 的缓冲区
 * @param fd 文件描述符
 * @return int 0: 成功, -1: 失败
 */
int connction_create(connection_t **conn, int fd);

/**
 * @brief 创建连接表
 * 
 * @param table 
 * @param capacity 
 * @return int 
 */
int connect_table_create(connect_table_t **table, int capacity);

/**
 * @brief 插入 connection 到连接表
 * 
 * @param table 连接表
 * @param conn 连接
 * @return int 0: 成功, -1: 失败
 */
int connect_table_insert(connect_table_t *table, connection_t *conn);

/**
 * @brief 从连接表中移除 fd 对应的连接
 * 
 * @param table 连接表
 * @param fd 文件描述符
 * @return int 0: 成功, -1: 失败
 */
int connect_table_remove(connect_table_t *table, int fd);

/**
 * @brief 从连接表中查找 fd 对应的连接
 * 
 * @param table 连接表
 * @param fd 文件描述符
 * @param conn 存储连接的缓冲区
 * @return int 0: 成功, -1: 失败
 */
int connect_table_find(connect_table_t *table, int fd, connection_t **conn);

#endif // CONNECT_TABLE_H