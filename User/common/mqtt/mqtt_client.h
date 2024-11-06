#ifndef _MQTT_CLIENT_H_
#define _MQTT_CLIENT_H_

#include <stdio.h>
#include <stdint.h>

struct config_t {
    char        *host;
    char        *client_id;
    char        *user;
    char        *pass;
    uint16_t    *port;
};

typedef struct {
    /// Atributes 
    void *net_if;    
    struct config_t config;
    /// Methods
    int32_t (*proc)(void* _this);
    int32_t (*sub)(void* _this, const char *_topic,
                    uint8_t _qos, uint8_t _retain);
    int32_t (*pub)(void* , const char *_topic, const char *_data, 
                    uint8_t _qos, uint8_t _retain);
} mqtt_clent_t;

mqtt_clent_t *mqtt_client_init(void *_net_if, struct config_t);

#endif