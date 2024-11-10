#ifndef _MQTT_CLIENT_H_
#define _MQTT_CLIENT_H_

#include <stdio.h>
#include <stdint.h>
#include "sm_elapsed_timer.h"

#define MQTT_CLIENT_POLLING_DATA_PERIOD	1500


typedef struct mqtt_client_event_fn mqtt_client_event_fn_t;
typedef struct mqtt_client mqtt_client_t ;
typedef struct mqtt_config mqtt_config_t;

struct mqtt_config {
    char        *host;
    char        *client_id;
    char        *user;
    char        *pass;
    uint16_t    port;
};

struct mqtt_client {
    /// Atributes 
    void *net_if;    
    mqtt_config_t config;
    elapsed_timer_t timeout;

    mqtt_client_event_fn_t 	*event_fn;
    void 					*event_arg;
    /// Methods
    int32_t (*sub)(mqtt_client_t* _this, const char *_topic);
    int32_t (*pub)(mqtt_client_t* , const char *_topic, const char *_data);
    int32_t (*connect)(mqtt_client_t* this);
    int32_t (*disconn)(mqtt_client_t* this);
} ;

struct mqtt_client_event_fn{
	void (*on_mqtt_connected)(void*arg);
	void (*on_mqtt_disconnected)(void*);
	void (*on_mqtt_recv_msg)(const char* topic, const char* data, void* arg);
};

mqtt_client_t *mqtt_client_init(void *_net_if, mqtt_config_t*);
int32_t mqtt_client_proc(mqtt_client_t* this);
int32_t mqtt_client_reg_event(mqtt_client_t* this, mqtt_client_event_fn_t* , void*);
#endif
