#ifndef _MQTT_CLIENT_H_
#define _MQTT_CLIENT_H_

#include <stdio.h>
#include <stdint.h>

typedef struct mqtt_client mqtt_client_t;
typedef struct mqtt_client_event_fn mqtt_client_event_fn_t;

typedef struct{
    char        *host;
    char        *client_id;
    char        *user;
    char        *pass;
    uint16_t    *port;
}config_t;

struct mqtt_client{
    /// Atributes 
    void            *net_if;
    config_t        *config;
    /// Methods
    int32_t (*connect)(mqtt_client_t* _this);
    int32_t (*disconn)(mqtt_client_t* _this);
    int32_t (*sub)(mqtt_client_t* _this, const char *_topic);
    int32_t (*pub)(mqtt_client_t* , const char *_topic, const char *_data);

    /// Signal
    mqtt_client_event_fn_t  *event_fn;
    void                    *event_arg;
};

struct mqtt_client_event_fn{
    void (*on_mqtt_recv_msg)(const char* topic, const char* payload, void *arg);
    void (*on_mqtt_disconnected)(void* arg);
    void (*on_mqtt_connected)(void *arg);
};

int32_t mqtt_client_init(mqtt_client_t* this, void *_net_if,config_t *cfg);
int32_t mqtt_client_proc(mqtt_client_t* _this);
int32_t mqtt_client_reg_event(mqtt_client_t* _this, mqtt_client_event_fn_t* fn, void *arg);
#endif