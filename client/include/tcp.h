#ifndef TCP_H
#define TCP_H
#include <sys/types.h>
#include <sys/socket.h>
#undef __USE_XOPEN2K
#define __USE_XOPEN2K
#include <netdb.h>

/**
 * @brief Establish a connection with the server
 * 
 * @param hostname Host name of server
 * @param service  Port number of the server
 * @return int    On success, return the socket file descriptor which is connected, otherwise -1
 */
int tcp_connect(const char *hostname, const char *service);

#endif  // TCP_H