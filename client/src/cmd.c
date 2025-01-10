#include <cmd.h>

int cmd_read(char * restrict buf, size_t size, const char * restrict prompt) {
    printf("%s", prompt);
    size_t len = 0;
    while (len < size - 1 && (buf[len] = getchar()) != '\n') {
        len++;
    }
    buf[len] = '\0';
    return len > 0 ? len : -1;
}

