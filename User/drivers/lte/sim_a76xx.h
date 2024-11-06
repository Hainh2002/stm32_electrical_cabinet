#ifndef SIM_A76XX_H
#define SIM_A76XX_H

#include "sm_modem.h"



typedef struct sim_a76xx            sim_a76xx_t;
typedef struct sim_a76xx_event_fn   sim_a76xx_event_fn_t;

struct sim_a76xx{
    sm_modem_t      *modem;
    uint8_t         sim_connected;
    uint8_t         reg_network;
    sim_a76xx_event_fn_t    *event_fn;
    void                    *event_arg;

    int32_t (*reset)(sim_a76xx_t* this);
    int32_t (*reinit)(sim_a76xx_t* this);

    int32_t (*network_open)(sim_a76xx_t* this);

    int32_t (*mqtt_open)(sim_a76xx_t* this,
                        const char* host,
                        uint32_t port);

    int32_t (*mqtt_close)(sim_a76xx_t* this);

    int32_t (*mqtt_connect)(sim_a76xx_t* this,
                            const char* client_id,
                            const char* user,
                            const char* pass);

    int32_t (*mqtt_disconnect)(sim_a76xx_t* this);

    int32_t (*mqtt_subscribe)(sim_a76xx_t* this, const char* topic);

    int32_t (*mqtt_publish)(sim_a76xx_t* this, const char* topic);
};

struct sim_a76xx_event_fn{
    void (*on_modem_inited)(void* arg);
    void (*on_mqtt_recieve_msg)(char* topic, char* payload, void *arg);
    void (*on_mqtt_disconnected)(void* arg);
    void (*on_mqtt_connected)(void *arg);
};

int32_t sim_a76xx_init(sim_a76xx_t* this);
int32_t sim_a76xx_proc(sim_a76xx_t* this);
int32_t sim_a76xx_reg_event(sim_a76xx_t* this, sim_a76xx_event_fn_t *fn, void *arg);

#endif