#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "file.h"
#include "tcp.h"

int sendfile(int connfd, const char *path)
{
    // 发送文件头
    hdr_t fileHdr;
    fileHdr.nameLength = strlen(path);
    fileHdr.filename = (byte *)path;
    struct stat st;
    if (stat(path, &st) == -1)
    {
        return -1;
    }
    fileHdr.filesize = st.st_size;

    if (sendhdr(connfd, &fileHdr) == -1)
    {
        return -1;
    }

    // 打开文件
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }

    // 建立私有文件映射：只读取文件内容，并不修改文件
    byte *fileContent = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (fileContent == MAP_FAILED)
    {
        return -1;
    }
    // 发送文件内容
    if (sendn(connfd, fileContent, st.st_size) == -1)
    {
        return -1;
    }
    // 解除映射
    munmap(fileContent, st.st_size);
    // 关闭文件
    if (close(fd) == -1)
    {
        return -1;
    }
    return 0;
}

int sendhdr(int connfd, const hdr_t *hdr)
{
    // 发送文件名长度
    if (sendn(connfd, &hdr->nameLength, sizeof(hdr->nameLength)) == -1)
    {
        return -1;
    }
    // 发送文件大小
    if (sendn(connfd, &hdr->filesize, sizeof(hdr->filesize)) == -1)
    {
        return -1;
    }
    // 发送文件名
    if (sendn(connfd, hdr->filename, hdr->nameLength) == -1)
    {
        return -1;
    }
    return 0;
}

int recvhdr(int sockfd, hdr_t *hdr) {
    if(hdr == NULL) {
        return -1;
    }
    // 接收文件名长度
    if(recvn(sockfd, &hdr->nameLength, sizeof(hdr->nameLength)) == -1) {
        return -1;
    }
    // 接收文件大小
    if(recvn(sockfd, &hdr->filesize, sizeof(hdr->filesize)) == -1) {
        return -1;
    }
    // 接收文件名
    if(recvn(sockfd, hdr->filename, hdr->nameLength) == -1) {
        return -1;
    }
    return 0;
}
