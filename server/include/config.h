#ifndef CONFIG_H
#define CONFIG_H

#include "toml.h"

struct server_config {
    char hostname[256];
    char service[256];
    int worker_number;
};

int load_server_config(const char *pathname, struct server_config *cfg);

#endif // CONFIG_H

