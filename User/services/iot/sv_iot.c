#include "sv_iot.h"
#include "sm_types.h"
#include "json-maker.h"
#include "tiny-json.h"
#include "sm_logger.h"
#include "sv_iot_define.h"

#define IOT_IDLE_TIMEOUT	15000
#define IOT_OPEN_TIMEOUT	5000
#define IOT_SUB_TIMEOUT		5000
#define IOT_SYNC_TIMEOUT	5000

#define JSON_OBJ_MAX		32

#define IOT_RETRY_CONNECT_MAX	5

static char* TAG = "SV_IOT";

static void sv_iot_on_conn(void* arg);
static void sv_iot_on_disc(void* arg);
static void sv_iot_on_msg(const char* topic, const char* data, void* arg);

static void sv_iot_on_config_msg(const char* msg, void* arg);
static void sv_iot_on_command_msg(const char* msg, void* arg);

static void sv_iot_build_err_response(char* buff, char* type, char* err_msg, int32_t err_code);

sv_iot_t g_sv_iot;

mqtt_client_event_fn_t g_mqtt_client_cb = {
	.on_mqtt_connected = sv_iot_on_conn,
	.on_mqtt_disconnected = sv_iot_on_disc,
	.on_mqtt_recv_msg = sv_iot_on_msg
};

sv_iot_t* sv_iot_create(mqtt_client_t	    *client,
						sv_iot_callback_t 	*event_cb,
						void				*event_arg){
	if (!client || !event_cb || !event_arg) return NULL;

	g_sv_iot.mqtt_client = client;
	g_sv_iot.event_cb = NULL;
	g_sv_iot.event_arg = NULL;
	g_sv_iot.sub_topic_num = 0;
	for (int i=0; i< IOT_SUB_TOPIC_NUMBER; i++){
		g_sv_iot.sub_topics->topic = NULL;
		g_sv_iot.sub_topics->topic_handle = NULL;
	}
	for (int i=0; i< IOT_PUB_QUEUE_SIZE; i++){
		g_sv_iot.pub_topics->topic = NULL;
		g_sv_iot.pub_topics->payload = NULL;
	}
	g_sv_iot.pub_head = 0;
	g_sv_iot.pub_tail = 0;
	g_sv_iot.state = IOT_STATE_IDLE;

	g_sv_iot.event_cb = event_cb;
	g_sv_iot.event_arg = event_arg;

	g_sv_iot.retry = 0;

	sv_iot_build_topic(g_sv_iot.device_name);

	g_sv_iot.sub_topics[0].topic = sv_iot_get_topic_cfg();
	g_sv_iot.sub_topics[0].topic_handle = sv_iot_on_config_msg;

	g_sv_iot.sub_topics[1].topic = sv_iot_get_topic_cmd();
	g_sv_iot.sub_topics[1].topic_handle = sv_iot_on_command_msg;

	g_sv_iot.sub_topic_num = 0;

	mqtt_client_reg_event(client, &g_mqtt_client_cb, &g_sv_iot);
	elapsed_timer_resetz(&g_sv_iot.timeout, IOT_IDLE_TIMEOUT);
	return &g_sv_iot;
}

int32_t sv_iot_get_state(sv_iot_t* this){
	return (int32_t) this->state;
}

int32_t sv_iot_push_data_to_cloud(sv_iot_t* this, char* topic, char* payload){
	if (!this || !topic || !payload) return -1;

	this->pub_topics[this->pub_head].topic = topic;
	memcpy(this->pub_topics[this->pub_head].payload, payload, strlen(payload));

	if (++this->pub_head == IOT_PUB_QUEUE_SIZE){
		this->pub_head = 0;
	}
	return 0;
}

int32_t sv_iot_process(sv_iot_t* this){
	if (!this) return -1;
	int32_t err = 0;
	mqtt_client_t *client = this->mqtt_client;
	switch (this->state){
	case IOT_STATE_IDLE:
		if (!elapsed_timer_get_remain(&this->timeout)){
			this->state = IOT_STATE_OPEN;
		}
		break;
	case IOT_STATE_OPEN:
		if (!elapsed_timer_get_remain(&this->timeout)){
			err = client->connect(client);
			if (err){
				if (++this->retry > IOT_RETRY_CONNECT_MAX){
					this->state = IOT_STATE_DISCONNECT;
				}else{
					elapsed_timer_resetz(&this->timeout, IOT_OPEN_TIMEOUT);
				}
			}else{
				this->state = IOT_STATE_CONNECT;
			}
		}
		break;
	case IOT_STATE_CONNECT:
		this->state = IOT_STATE_SUBSCRIBE;
		this->retry = 0;
		elapsed_timer_resetz(&this->timeout, IOT_SUB_TIMEOUT);
		break;
	case IOT_STATE_SUBSCRIBE:
		if (!elapsed_timer_get_remain(&this->timeout)){
			err = 0;
			if (this->sub_topics[this->sub_topic_num].topic != NULL){
				err = client->sub(client, this->sub_topics[this->sub_topic_num].topic);
			}
			if (err) {
				if (++this->retry > IOT_RETRY_CONNECT_MAX){
					this->state = IOT_STATE_DISCONNECT;
				}
			}else{
				if (++this->sub_topic_num == IOT_SUB_TOPIC_NUMBER){
					this->state = IOT_STATE_RUNNING;
				}
			}
		}
		break;
	case IOT_STATE_RUNNING:
		err = mqtt_client_proc(client);
		if (err){
			sv_iot_on_disc(this);
		}else{
			if (this->event_cb->on_sync && !elapsed_timer_get_remain(&this->timeout)){
				char sync_buff[IOT_PAYLOAD_SIZE];
				memset(sync_buff, 0, IOT_PAYLOAD_SIZE);
				this->event_cb->on_sync(sync_buff, this->event_arg);
				sv_iot_push_data_to_cloud(this, sv_iot_get_topic_update(), sync_buff);
			}
		}
		if (this->pub_tail != this->pub_head){
			err = client->pub(client,
							this->pub_topics[this->pub_tail].topic,
							this->pub_topics[this->pub_tail].payload);
			if (err) {
				/// LOG_ERR
			}
			this->pub_topics[this->pub_tail].topic = NULL;
			this->pub_topics[this->pub_tail].payload = NULL;
			if (++this->pub_tail == IOT_PUB_QUEUE_SIZE){
				this->pub_tail = 0;
			}
		}
		break;
	case IOT_STATE_DISCONNECT:
		elapsed_timer_resetz(&this->timeout, IOT_IDLE_TIMEOUT);
		client->disconn(client);
		this->state = IOT_STATE_IDLE;
		this->sub_topic_num = 0;
		if (this->event_cb->on_disc){
			this->event_cb->on_disc(this->event_arg);
		}
		break;
	}
	return 0;
}

static void sv_iot_on_conn(void* arg){
	sv_iot_t* this = (sv_iot_t*)arg;
	if (this->event_cb->on_conn){
		this->event_cb->on_conn(this->event_arg);
	}
}

static void sv_iot_on_disc(void* arg){
	sv_iot_t* this = (sv_iot_t*)arg;
	this->state = IOT_STATE_DISCONNECT;
}

static void sv_iot_on_msg(const char* topic, const char* data, void* arg){
	sv_iot_t* this = (sv_iot_t*)arg;
	for (int i = 0; i < this->sub_topic_num; i++){
		if (this->sub_topics[i].topic != NULL &&
			! strncmp(this->sub_topics[i].topic, topic, strlen(topic))){
			this->sub_topics[i].topic_handle(data, arg);
		}
	}
}

static void sv_iot_on_config_msg(const char* data, void* arg){
	json_t mem[JSON_OBJ_MAX];
	const json_t* json = json_create((char*)data, mem, ARRAY_SIZE(mem));

	if (!json) return;


}

static void sv_iot_on_command_msg(const char* data, void* arg){
	sv_iot_t* this = (sv_iot_t*) arg;

	json_t mem[JSON_OBJ_MAX];
	const json_t* json = json_create((char*)data, mem, ARRAY_SIZE(mem));

	if (!json) {
		LOG_ERR(TAG, "Format CMD msg is invalid");
		char payload_buff[128];
		sv_iot_build_err_response(payload_buff, "CMD_RES", "Format CMD msg is invalid", -1);
		sv_iot_push_data_to_cloud(this,	sv_iot_get_topic_cmd_res(),	payload_buff);
		return;
	}

	const json_t *type_msg = json_getProperty(json, "type");
	if (type_msg && !strcmp(json_getValue(type_msg), "CMD")) {
		// phase 1 cmd
		const json_t *phase1 = json_getProperty(json, "phase_1");
		if (phase1 && !strcmp(json_getValue(phase1), "relay_on")) {
			this->event_cb->on_cmd(PHASE_1_CMD_ON, NULL, this->event_arg);
		}
		if (phase1 && !strcmp(json_getValue(phase1), "relay_off")) {
			this->event_cb->on_cmd(PHASE_1_CMD_OFF, NULL, this->event_arg);
		}
		// phase 2 cmd
		const json_t *phase2 = json_getProperty(json, "phase_2");
		if (phase2 && !strcmp(json_getValue(phase2), "relay_on")) {
			this->event_cb->on_cmd(PHASE_2_CMD_ON, NULL, this->event_arg);
		}
		if (phase2 && !strcmp(json_getValue(phase2), "relay_off")) {
			this->event_cb->on_cmd(PHASE_2_CMD_OFF, NULL, this->event_arg);
		}
		// phase 3 cmd
		const json_t *phase3 = json_getProperty(json, "phase_3");
		if (phase3 && !strcmp(json_getValue(phase3), "relay_on")) {
			this->event_cb->on_cmd(PHASE_3_CMD_ON, NULL, this->event_arg);
		}
		if (phase3 && !strcmp(json_getValue(phase3), "relay_off")) {
			this->event_cb->on_cmd(PHASE_3_CMD_OFF, NULL, this->event_arg);
		}
		// cabinet cmd
		const json_t *cabinet = json_getProperty(json, "cabinet");
		if (cabinet && !strcmp(json_getValue(cabinet), "active")) {
			this->event_cb->on_cmd(CABINET_CMD_ACTIVE, NULL, this->event_arg);
		}
		if (cabinet && !strcmp(json_getValue(cabinet), "deactive")) {
			this->event_cb->on_cmd(CABINET_CMD_DEACTIVE, NULL, this->event_arg);
		}

	}else {
		char payload_buff[128];
		sv_iot_build_err_response(payload_buff, "CMD_RES", "CMD is not supported", -2);
		sv_iot_push_data_to_cloud(this,	sv_iot_get_topic_cmd_res(),	payload_buff);
	}

}

static void sv_iot_build_err_response(char* buff, char* type, char* err_msg, int32_t err_code) {
	char* p = buff;

	p = json_objOpen(p, NULL);
	p = json_str(p, "type", type);
	p = json_str(p, "err_msg", err_msg);
	p = json_int(p, "err_code", err_code);
	p =json_objClose(p);
	p = json_end(p);
}