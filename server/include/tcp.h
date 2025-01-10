#ifndef TCP_H
#define TCP_H

#include <sys/types.h>
#include <sys/socket.h>
#undef __USE_XOPEN2K
#define __USE_XOPEN2K
#include <netdb.h>


/**
 * @brief Create a TCP/IPv4 socket and bind it to the given hostname and service
 * 
 * @param hostname The hostname, which can be an IP address or a domain name
 * @param service The service, which can be a port number or a service name
 * @return int on success, the file descriptor of the socket; on failure, -1
 */
int tcp_listen(const char *hostname, const char *service);

#endif // TCP_H

