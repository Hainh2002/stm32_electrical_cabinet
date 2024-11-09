#include "mqtt_client.h"
#include "simcom_a76xx.h"

static int32_t mqtt_subscribe(mqtt_clent_t* this, const char *_topic, 
                              uint8_t _qos, uint8_t _retain);
static int32_t mqtt_publish(mqtt_clent_t* this, const char *_topic, const char *_data, 
                            uint8_t _qos, uint8_t _retain);
static int32_t mqtt_proccess(mqtt_clent_t* this);

mqtt_clent_t *mqtt_client_init(void *_net_if, const char* _host, const char* _port){
    mqtt_client_t* this = malloc(sizeof(mqtt_client_t));
    this->net_if = _net_if;
    this->config->host = _host;
    this->config->port = _port;
    this->sub = mqtt_subscribe;
    this->pub = mqtt_publish;
    this->proc = mqtt_proccess;
    return this->base;
}



static int32_t mqtt_subscribe(mqtt_clent_t* , const char *_topic, uint8_t _qos, uint8_t _retain){
    /// TODO: Publish mqtt data.
}

static int32_t mqtt_publish(mqtt_clent_t* , const char *_topic, const char *_data, uint8_t _qos, uint8_t _retain){
    /// TODO: Publish mqtt data.
}
static int32_t mqtt_proccess(mqtt_clent_t* this){

}