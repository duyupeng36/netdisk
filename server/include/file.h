#ifndef FILE_H
#define FILE_H

#include <stdbool.h>



typedef unsigned char byte;

typedef struct {
    size_t nameLength;    // 文件名长度
    off_t filesize;   // 文件大小
    byte *filename;   // 文件名
} hdr_t;

/**
 * @brief Send file
 * 
 * @param fd Connection socket file descriptor
 * @param path file path
 * @return on success return 0, otherwise return -1
 */
int sendfile(int fd, const char *path);


/**
 * @brief Send file header
 * 
 * @param fd Connection socket file descriptor
 * @param hdr contains the data to be sent
 * @return on success return 0, otherwise return -1 
 */
int sendhdr(int fd, const hdr_t *hdr);

/**
 * @brief receive file header
 * 
 * @param sockfd SOCKET file descriptor
 * @param hdr store the file header information buffer
 * @return on success return 0, otherwise return -1
 */
int recvhdr(int sockfd, hdr_t *hdr);
#endif // FILE_H
