/*
 * sv_iot_topic.c
 *
 *  Created on: Nov 10, 2024
 *      Author: Ng Thuy Quynh
 */
#include "sv_iot_topic.h"

static char* g_topic_update = NULL;
static char* g_topic_config = NULL;
static char* g_topic_cfg_res = NULL;
static char* g_topic_cmd = NULL;
static char* g_topic_cmd_res = NULL;

void sv_iot_build_topic(const char* SN) {
    char buff[64];
    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_UPDATE, SN);
    g_topic_update = (char*)malloc(64);
    memcpy(g_topic_update, buff, 64);

    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_CONFIG, SN);
    g_topic_config = (char*)malloc(64);
    memcpy(g_topic_config, buff, 64);

    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_CONFIG_RES, SN);
    g_topic_cfg_res = (char*)malloc(64);
    memcpy(g_topic_cfg_res, buff, 64);

    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_COMMAND, SN);
    g_topic_cmd = (char*)malloc(64);
    memcpy(g_topic_cmd, buff, 64);

    memset(buff, 0, 64);
    sprintf(buff, SV_IOT_TOPIC_COMMAND_RES, SN);
    g_topic_cmd_res = (char*)malloc(64);
    memcpy(g_topic_cmd_res, buff, 64);
}

char* sv_iot_get_topic_update() {
    return g_topic_update;
}
char* sv_iot_get_topic_cfg() {
    return g_topic_config;
}
char* sv_iot_get_topic_cfg_res() {
    return g_topic_cfg_res;
}
char* sv_iot_get_topic_cmd() {
    return g_topic_cmd;
}
char* sv_iot_get_topic_cmd_res() {
    return g_topic_cmd_res;
}

