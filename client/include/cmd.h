#ifndef CMD_H
#define CMD_H

#include <stdio.h>

/**
 * @brief Read command from standard input
 * 
 * @param buf A buffer for storing commands.
 * @param size The size of the buffer.
 * @param prompt The prompt message.
 * @return int on success, return number of characters read. On error, return -1.
 */
int cmd_read(char * restrict buf, size_t size, const char * restrict prompt);

#endif // CMD_H
