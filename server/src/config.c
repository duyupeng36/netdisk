#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

int load_server_config(const char *pathname, struct server_config *cfg) {
    FILE *fp = fopen(pathname, "r");
    if (fp == NULL) {
        return -1;
    }

    char errbuf[200];
    toml_table_t *conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    if (conf == NULL) {
        return -1;
    }
    toml_table_t *server = toml_table_in(conf, "server");
    if (server == NULL) {
        return -1;
    }
    // 读取 hostname
    toml_datum_t hostname = toml_string_in(server, "hostname");
    if (hostname.ok) {
        strncpy(cfg->hostname, hostname.u.s, sizeof(cfg->hostname));
    } else {
        return -1;
    }
    // 读取 service
    toml_datum_t service = toml_string_in(server, "service");
    if (service.ok) {
        strncpy(cfg->service, service.u.s, sizeof(cfg->service));
    } else {
        return -1;
    }
    // 读取 worker_number
    toml_datum_t worker_number = toml_int_in(server, "worker_number");
    if (worker_number.ok) {
        cfg->worker_number = worker_number.u.i;
    } else {
        return -1;
    }

    return 0;
}

