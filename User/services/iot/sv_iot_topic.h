#ifndef SV_IOT_TOPIC_H
#define SV_IOT_TOPIC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SV_IOT_TOPIC_UPDATE                 "cab/%s/update"
#define SV_IOT_TOPIC_CONFIG                 "cab/%s/config"
#define SV_IOT_TOPIC_CONFIG_RES             "cab/%s/config_response"
#define SV_IOT_TOPIC_COMMAND                "cab/%s/command"
#define SV_IOT_TOPIC_COMMAND_RES            "cab/%s/command_response"

char* g_topic_update = NULL;
char* g_topic_config = NULL;
char* g_topic_cfg_res = NULL;
char* g_topic_cmd = NULL;
char* g_topic_cmd_res = NULL;

static inline void sv_iot_build_topic(const char* SN) {
    char buff[64];
    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_UPDATE, SN);
    g_topic_update = (char*)malloc(strlen(buff));
    memcpy(g_topic_update, buff, strlen(buff));

    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_CONFIG, SN);
    g_topic_config = (char*)malloc(strlen(buff));
    memcpy(g_topic_config, buff, strlen(buff));

    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_CONFIG_RES, SN);
    g_topic_cfg_res = (char*)malloc(strlen(buff));
    memcpy(g_topic_cfg_res, buff, strlen(buff));

    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_COMMAND, SN);
    g_topic_cmd = (char*)malloc(strlen(buff));
    memcpy(g_topic_cmd, buff, strlen(buff));

    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_COMMAND_RES, SN);
    g_topic_cmd_res = (char*)malloc(strlen(buff));
    memcpy(g_topic_cmd_res, buff, strlen(buff));
}

static inline char* sv_iot_get_topic_update() {
    return g_topic_update;
}
static inline char* sv_iot_get_topic_cfg() {
    return g_topic_config;
}
static inline char* sv_iot_get_topic_cfg_res() {
    return g_topic_cfg_res;
}
static inline char* sv_iot_get_topic_cmd() {
    return g_topic_cmd;
}
static inline char* sv_iot_get_topic_cmd_res() {
    return g_topic_cmd_res;
}
#endif //SV_IOT_TOPIC_H
