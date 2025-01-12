#ifndef AUTH_H
#define AUTH_H

/**
 * @brief Login to the server
 * 
 * @param fd The file descriptor of the socket.
 * @return int On success, return 0. On error, return -1.
 */
int login(int fd);
#endif // AUTH_H