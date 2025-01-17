#include <stdio.h>
#include <string.h>

#include <json-c/json.h>

#include "message.h"

#include "response.h"


int response_mkdir(int fd, struct state *state) {
    char *message = NULL;
    if(message_recv(fd, &message) == -1) {
        fprintf(stderr, "Failed to receive message\n");
        return -1;
    }
    json_object *root = json_tokener_parse(message);
    if(root == NULL) {
        fprintf(stderr, "Failed to parse message\n");
        return -1;
    }
    enum json_type type = json_object_get_type(root);
    if(type != json_type_object) {
        fprintf(stderr, "Invalid message\n");
        return -1;
    }
    // 获取 type
    json_object *type_obj = json_object_object_get(root, "type");
    if(type_obj == NULL) {
        fprintf(stderr, "Failed to get type\n");
        return -1;
    }
    if(strcmp(json_object_get_string(type_obj), "error") == 0) {
        // 获取错误信息
        json_object *error = json_object_object_get(root, "message");
        if(error == NULL) {
            fprintf(stderr, "Failed to get error\n");
            return -1;
        }
        fprintf(stderr, "Error: %s\n", json_object_get_string(error));
        return -1;
    }
    return 0;
}

int response_rmdir(int fd, struct state *state) {
    char *message = NULL;
    if(message_recv(fd, &message) == -1) {
        fprintf(stderr, "Failed to receive message\n");
        return -1;
    }
    json_object *root = json_tokener_parse(message);
    if(root == NULL) {
        fprintf(stderr, "Failed to parse message\n");
        return -1;
    }
    enum json_type type = json_object_get_type(root);
    if(type != json_type_object) {
        fprintf(stderr, "Invalid message\n");
        return -1;
    }
    // 获取 type
    json_object *type_obj = json_object_object_get(root, "type");
    if(type_obj == NULL) {
        fprintf(stderr, "Failed to get type\n");
        return -1;
    }
    if(strcmp(json_object_get_string(type_obj), "error") == 0) {
        // 获取错误信息
        json_object *error = json_object_object_get(root, "message");
        if(error == NULL) {
            fprintf(stderr, "Failed to get error\n");
            return -1;
        }
        fprintf(stderr, "Error: %s\n", json_object_get_string(error));
        return -1;
    }
    return 0;
}


int response_ls(int fd, struct state *state) {
    char *message = NULL;
    if(message_recv(fd, &message) == -1) {
        fprintf(stderr, "Failed to receive message\n");
        return -1;
    }
    json_object *root = json_tokener_parse(message);
    if(root == NULL) {
        fprintf(stderr, "Failed to parse message\n");
        return -1;
    }
    enum json_type type = json_object_get_type(root);
    if(type != json_type_object) {
        fprintf(stderr, "Invalid message\n");
        return -1;
    }
    // 获取 type
    json_object *type_obj = json_object_object_get(root, "type");
    if(type_obj == NULL) {
        fprintf(stderr, "Failed to get type\n");
        return -1;
    }
    if(strcmp(json_object_get_string(type_obj), "error") == 0) {
        // 获取错误信息
        json_object *error = json_object_object_get(root, "message");
        if(error == NULL) {
            fprintf(stderr, "Failed to get error\n");
            return -1;
        }
        fprintf(stderr, "Error: %s\n", json_object_get_string(error));
        return -1;
    }
    // 获取成功信息 files
    json_object *files = json_object_object_get(root, "files");
    if(files == NULL) {
        fprintf(stderr, "Failed to get files\n");
        return -1;
    }
    printf("%s\n", json_object_get_string(files));
    return 0;
}

int response_cd(int fd, struct state *state) {
    char *message = NULL;
    if(message_recv(fd, &message) == -1) {
        fprintf(stderr, "Failed to receive message\n");
        return -1;
    }
    printf("%s\n", message);
    json_object *root = json_tokener_parse(message);
    if(root == NULL) {
        fprintf(stderr, "Failed to parse message\n");
        return -1;
    }  
    enum json_type type = json_object_get_type(root);
    if(type != json_type_object) {
        fprintf(stderr, "Invalid message\n");
        return -1;
    }

    // 获取 type
    json_object *type_obj = json_object_object_get(root, "type");
    if(type_obj == NULL) {
        fprintf(stderr, "Failed to get type\n");
        return -1;
    }
    if(strcmp(json_object_get_string(type_obj), "error") == 0) {
        // 获取错误信息
        json_object *error = json_object_object_get(root, "message");
        if(error == NULL) {
            fprintf(stderr, "Failed to get error\n");
            return -1;
        }
        fprintf(stderr, "Error: %s\n", json_object_get_string(error));
        return -1;
    }
    // 获取成功信息 cwd
    json_object *cwd = json_object_object_get(root, "cwd");
    if(cwd == NULL) {
        fprintf(stderr, "Failed to get cwd\n");
        return -1;
    }
    strncpy(state->cwd, json_object_get_string(cwd), sizeof(state->cwd));
    return 0;
}


int response_pwd(int fd, struct state *state) {
    //读取信息
    char *message = NULL;
    if(message_recv(fd, &message) == -1) {
        fprintf(stderr, "Failed to receive message\n");
        return -1;
    }
    json_object *root = json_tokener_parse(message);
    if(root == NULL) {
        fprintf(stderr, "Failed to parse message\n");
        return -1;
    }
    enum json_type type = json_object_get_type(root);
    if(type != json_type_object) {
        fprintf(stderr, "Invalid message\n");
        return -1;
    }
    // 获取 type
    json_object *type_obj = json_object_object_get(root, "type");
    if(type_obj == NULL) {
        fprintf(stderr, "Failed to get type\n");
        return -1;
    }
    if(strcmp(json_object_get_string(type_obj), "error") == 0) {
        // 获取错误信息
        json_object *error = json_object_object_get(root, "message");
        if(error == NULL) {
            fprintf(stderr, "Failed to get error\n");
            return -1;
        }
        fprintf(stderr, "Error: %s\n", json_object_get_string(error));
        return -1;
    }
    // 获取成功信息 cwd
    json_object *cwd = json_object_object_get(root, "cwd");
    if(cwd == NULL) {
        fprintf(stderr, "Failed to get cwd\n");
        return -1;
    }
    printf("%s\n", json_object_get_string(cwd));
    return 0;
}


int response_gets(int fd, struct state *state) {
    return 0;
}


int response_puts(int fd, struct state *state) {
    return 0;
}

int response_remove(int fd, struct state *state) {
    return 0;
}
