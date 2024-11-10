#include "mqtt_client.h"
#include "sim_a76xx.h"

static int32_t mqtt_subscribe(mqtt_client_t* this, const char *_topic);
static int32_t mqtt_publish(mqtt_client_t* this, const char *_topic, const char *_data);
static int32_t mqtt_connect(mqtt_client_t* this);
static int32_t mqtt_disconnect(mqtt_client_t* this);

static void on_mqtt_disc(void* arg);
static void on_mqtt_msg(char* topic, char* payload, void *arg);

sim_a76xx_event_fn_t g_sim_module_event = {
		.on_mqtt_disconnected = on_mqtt_disc,
		.on_mqtt_recieve_msg = on_mqtt_msg
};
mqtt_client_t *mqtt_client_init(void *_net_if, mqtt_config_t* cfg){
    mqtt_client_t* this = malloc(sizeof(mqtt_client_t));
    this->net_if = _net_if;
    this->config.client_id = cfg->client_id;
    this->config.host = cfg->host;
    this->config.port = cfg->port;
    this->config.user = cfg->user;
    this->config.pass = cfg->pass;
//    memcpy(&this->config, cfg, sizeof(this->config));
    this->sub = mqtt_subscribe;
    this->pub = mqtt_publish;
    this->connect = mqtt_connect;
    this->disconn = mqtt_disconnect;

    sim_a76xx_reg_event(_net_if, &g_sim_module_event, this);
    elapsed_timer_resetz(&this->timeout, MQTT_CLIENT_POLLING_DATA_PERIOD);
    return this;
}

int32_t mqtt_client_reg_event(mqtt_client_t* this, mqtt_client_event_fn_t* _fn, void* _arg){
	this->event_fn = _fn;
	this->event_arg = _arg;
}



static int32_t mqtt_subscribe(mqtt_client_t* this, const char *_topic){
	sim_a76xx_t* sim_module = (sim_a76xx_t*)this->net_if;
	return sim_module->mqtt_subscribe(sim_module, _topic);
}

static int32_t mqtt_publish(mqtt_client_t* this, const char *_topic, const char *_data){
	sim_a76xx_t* sim_module = (sim_a76xx_t*)this->net_if;
	return sim_module->mqtt_publish(sim_module, _topic, _data);
}

static int32_t mqtt_connect(mqtt_client_t* this){
	sim_a76xx_t* sim_module = (sim_a76xx_t*)this->net_if;
	int32_t err = sim_module->mqtt_open(sim_module, this->config.client_id);
	if (err) return -1;
	err = sim_module->mqtt_connect(sim_module,
									this->config.host,
									this->config.port,
									this->config.user,
									this->config.pass);
	return err;
}

static int32_t mqtt_disconnect(mqtt_client_t* this){
	sim_a76xx_t* sim_module = (sim_a76xx_t*)this->net_if;
	sim_module->mqtt_disconnect(sim_module);
	sim_module->mqtt_close(sim_module);
	return 0;
}

int32_t mqtt_client_proc(mqtt_client_t* this){
	if (!this) return -1;
	sim_a76xx_t* sim_module = (sim_a76xx_t*)this->net_if;
	if (!elapsed_timer_get_remain(&this->timeout)){
		int32_t err = sim_a76xx_proc(sim_module);
		elapsed_timer_reset(&this->timeout);
		return err;
	}else{
		return 0;
	}

}

static void on_mqtt_disc(void* arg){
	mqtt_client_t* this = (mqtt_client_t*) arg;
	this->event_fn->on_mqtt_disconnected(this->event_arg);
	this->disconn(this);
}
static void on_mqtt_msg(char* topic, char* payload, void *arg){
	mqtt_client_t* this = (mqtt_client_t*) arg;
	this->event_fn->on_mqtt_recv_msg(topic, payload, this->event_arg);
}
