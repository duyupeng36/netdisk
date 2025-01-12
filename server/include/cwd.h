#ifndef CWD_H
#define CWD_H



typedef struct cwd cwd_t;

struct cwd
{
    char **path;
    int top;
    int size;
};

/**
 * @brief create a current working directory object
 * 
 * @param size How many directories can be stored in the cwd
 * @return cwd_t* A pointer to the cwd object, NULL if failed
 */
cwd_t *cwd_create(int size);

/**
 * @brief Destroy the cwd object
 * 
 * @param cwd The cwd object to destroy
 */
void cwd_destroy(cwd_t *cwd);

/**
 * @brief Push a directory to the cwd
 * 
 * @param cwd cwd object
 * @param path The directory to push
 * @return int 0 if success, -1 if failed
 */
int cwd_push(cwd_t *cwd, const char *path);

/**
 * @brief Pop a directory from the cwd
 * 
 * @param cwd cwd object
 * @return int 0 if success, -1 if failed
 */
int cwd_pop(cwd_t *cwd);

/**
 * @brief Get the current directory
 * 
 * @param cwd cwd object
 * @return const char* The current directory
 */
const char *cwd_get(cwd_t *cwd);

#endif // CWD_H