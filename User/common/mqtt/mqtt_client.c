#include "mqtt_client.h"
#include "sim_a76xx.h"

static int32_t mqtt_subscribe(mqtt_client_t* this, const char *_topic, 
                              uint8_t _qos, uint8_t _retain);
static int32_t mqtt_publish(mqtt_client_t* this, const char *_topic, const char *_data, 
                            uint8_t _qos, uint8_t _retain);
static int32_t mqtt_proccess(mqtt_client_t* this);

int32_t mqtt_client_init(mqtt_client_t* this, void *_net_if,config_t *cfg){
    this->net_if = _net_if;
    this->config = cfg;
    this->sub = mqtt_subscribe;
    this->pub = mqtt_publish;
    return 0;
}



static int32_t mqtt_subscribe(mqtt_client_t* , const char *_topic, uint8_t _qos, uint8_t _retain){
    /// TODO: Publish mqtt data.
}

static int32_t mqtt_publish(mqtt_client_t* , const char *_topic, const char *_data, uint8_t _qos, uint8_t _retain){
    /// TODO: Publish mqtt data.
}
static int32_t mqtt_proccess(mqtt_client_t* this){

}