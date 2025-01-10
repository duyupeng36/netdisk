#include <string.h>
#include <stdlib.h>

#include "config.h"


int load_config(const char *path, struct config *config, enum cfgtype cfgtype) {
    FILE *file = fopen(path, "r");
    if(file == NULL) {
        fprintf(stderr, "Failed to open config file: %s\n", path);
        return 1;
    }

    char errbuf[200];
    
    toml_table_t *toml = toml_parse_file(file,errbuf, sizeof(errbuf));
    fclose(file);

    if(config == NULL) {
        fprintf(stderr, "Failed to parse config file: %s\n", path);
        return 1;
    }

    switch (cfgtype)
    {
    case CFG_SERVER:
        // 获取 [server] 下的配置
        toml_table_t *server = toml_table_in(toml, "server");
        if(server == NULL) {
            fprintf(stderr, "Failed to get [server] section\n");
            return -1;
        }
        // 获取 hostname
        toml_datum_t hostname = toml_string_in(server, "hostname");
        if(hostname.ok) {
            strncpy(
                ((struct server_config *)config)->hostname,
                strlen(hostname.u.s) ? hostname.u.s: HOSTNAME, 
                16
            );
            free(hostname.u.s);
        } else {
            strncpy(
                ((struct server_config *)config)->hostname, 
                HOSTNAME, 
                16
            );
        }

        // 获取 service
        toml_datum_t service = toml_string_in(server, "service");
        if(service.ok) {
            strncpy(
                ((struct server_config *)config)->service, 
                strlen(service.u.s) ? service.u.s:SERVICE, 
                6
            );
            free(service.u.s);
        } else {
            strncpy(
                ((struct server_config *)config)->service,
                SERVICE,
                6
            );
        }
        // 获取 worker_number
        toml_datum_t worker_number = toml_int_in(server, "worker_number");
        if(worker_number.ok) {
            ((struct server_config *)config)->worker_number = worker_number.u.i ? worker_number.u.i : WORKER_NUMBER;
        } else {
            ((struct server_config *)config)->worker_number = WORKER_NUMBER;
        }
        break;
    default:
        fprintf(stderr, "Unknown config type\n");
        return -1;
        break;
    }

    return 0;
}
