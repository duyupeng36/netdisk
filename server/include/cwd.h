#ifndef CWD_H
#define CWD_H

#define PATH_NODE_MAX 32

typedef struct path_node path_node_t;

struct path_node {
    char path[PATH_NODE_MAX];
    path_node_t *next;
    path_node_t *prev; 
};

typedef struct {
    path_node_t *top;
    path_node_t *bottom;
} cwd_t;

/**
 * @brief 创建路径节点
 * 
 * @param node 存储节点的缓冲区
 * @param path 当前节点的路径名
 * @return int 0: 成功, -1: 失败
 */
int cwd_create_path_node(path_node_t **node, const char *path);

/**
 * @brief 初始化当前工作目录栈
 * 
 * @param cwd 当前工作目录
 * @return int 0: 成功, -1: 失败
 */
int cwd_create(cwd_t **cwd);

/**
 * @brief 清空当前工作目录栈
 * 
 * @param cwd 当前工作目录栈
 * @return int 0: 成功, -1: 失败
 */
int cwd_clear(cwd_t *cwd);

/**
 * @brief 添加 path 到当前工作目录
 * 
 * @param cwd 当前工作目录栈
 * @param path 路径： /a/b/c 或 a/b/c 或 a
 * @return int 0: 成功, -1: 失败
 */
int cwd_push(cwd_t *cwd, char *path);

/**
 * @brief 弹出当前工作目录栈的最后一个路径
 * 
 * @param cwd 当前工作目录栈
 * @return int 0: 成功, -1: 失败
 */
int cwd_pop(cwd_t *cwd);

/**
 * @brief 获取当前工作目录
 * 
 * @param cwd 当前工作目录栈
 * @param buf 存储当前工作目录字符串的缓冲区
 * @param size 缓冲区大小
 * @return int 0: 成功, -1: 失败
 */
int cwd_pwd(cwd_t *cwd, char *buf, size_t size);

#endif // CWD_H

