#ifndef EPOLL_H
#define EPOLL_H
#include <sys/types.h>
#include <sys/epoll.h>


/**
 * @brief Add fd to the epoll instance to listen for events events
 * 
 * @param epfd File descriptor for the epoll instance
 * @param data Data to associate with the fd
 * @param events Events to listen fo
 * @return on success return 0, otherwise return -1
 */
int epoll_add(int epfd, int fd, uint32_t events);

/**
 * @brief delete fd from the epoll instance
 * 
 * @param epfd File descriptor for the epoll instance
 * @param fd The file descriptor to delete
 * @return on success return 0, otherwise return -1
 */
int epoll_del(int epfd, int fd);


#endif