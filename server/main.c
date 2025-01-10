#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <config.h>

#define CONFIG_PATH "config/config.toml"


int main(int argc, char *argv[]) {

    if(argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s <config file>\n", argv[0]);
        return 1;
    }

    char *config_path = argc > 2 ? argv[1] : CONFIG_PATH;

    struct server_config server_config;
    server_config.hostname = malloc(sizeof(char) * 16);
    server_config.service = malloc(sizeof(char) * 6); 
    if(load_config(config_path, (struct config *)&server_config, CFG_SERVER) != 0) {
        fprintf(stderr, "Failed to load config\n");
        return 1;
    }

    printf("Server config:\n");
    printf("hostname: %s\n", server_config.hostname);
    printf("service: %s\n", server_config.service);
    printf("worker_number: %ld\n", server_config.worker_number);
    return 0;
}

