#ifndef CONFIG_H
#define CONFIG_H

#include <sys/sysinfo.h>

#include "toml.h"

#define HOSTNAME "127.0.0.1"
#define SERVICE "8080"
#define ROOT "/home/dyp/netdisk"
#define WORKER_NUMBER (2 * get_nprocs_conf())

enum cfgtype {
    CFG_SERVER,
};

struct config {
    unsigned char _pad[1024];
};

struct server_config {
    char *hostname;
    char *service;
    char *root;
    long worker_number;
};


/**
 * @brief Load config from file, and parse it into a struct
 * 
 * @param path where the config file is located
 * @param config the struct to store the config
 * @return int 0 if success, -1 if failed
 */
int load_config(const char *path, struct config *config, enum cfgtype cfgtype);

#endif // CONFIG_H