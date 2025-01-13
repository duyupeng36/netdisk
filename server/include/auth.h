#ifndef AUTH_H
#define AUTH_H
#include <pthread.h>

#include "cwd.h"

typedef struct user user_t;
typedef enum {
    USER_LOINGED,  // 已登录
    USER_UNLOGIN,  // 未登录
    USER_ACTIVE,   // 活跃
    USER_UNACTIVE  // 不活跃
} user_status_t;

struct user {
    int fd;
    char *username;
    cwd_t *cwd;
    user_status_t status;
    user_t *next;
};

typedef struct user_table {
    user_t **users;
    int size;
    int capacity;
} user_table_t;

/**
 * @brief Initialize a user table
 * 
 * @param capacity user table capacity
 * @return user_table_t* user table, NULL on error
 */
user_table_t *user_table_init(int capacity);

/**
 * @brief get user by fd
 * 
 * @param table the user table
 * @param fd the file descriptor of the user
 * @return user_t* the user, NULL if not found
 */
user_t *user_table_get(user_table_t *table, int fd);

/**
 * @brief Add a user to the user table
 * 
 * @param table the user table
 * @param fd the file descriptor of the user
 * @return int 0 on success, -1 on error
 */
int user_table_add(user_table_t *table, int fd);


#endif // AUTH_H