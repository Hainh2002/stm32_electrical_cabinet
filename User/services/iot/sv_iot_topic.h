#ifndef SV_IOT_TOPIC_H
#define SV_IOT_TOPIC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SV_IOT_TOPIC_UPDATE                 "electric_cabinet/update/%s"
#define SV_IOT_TOPIC_CONFIG                 "electric_cabinet/config/%s"
#define SV_IOT_TOPIC_CONFIG_RES             "electric_cabinet/config_response/%s"
#define SV_IOT_TOPIC_COMMAND                "electric_cabinet/command/%s"
#define SV_IOT_TOPIC_COMMAND_RES            "electric_cabinet/command_response/%s"

void sv_iot_build_topic(const char* SN) ;
char* sv_iot_get_topic_update() ;
char* sv_iot_get_topic_cfg() ;
char* sv_iot_get_topic_cfg_res() ;
char* sv_iot_get_topic_cmd() ;
char* sv_iot_get_topic_cmd_res() ;
#endif //SV_IOT_TOPIC_H
