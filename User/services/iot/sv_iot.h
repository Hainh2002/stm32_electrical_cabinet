#ifndef SV_IOT_H
#define SV_IOT_H

#include "mqtt_client.h"
#include "sm_elapsed_timer.h"
#include "sv_iot_topic.h"

#define IOT_SUB_TOPIC_NUMBER 	4
#define IOT_PUB_QUEUE_SIZE		4
#define IOT_PAYLOAD_SIZE		1024
/// EVENT
typedef enum {
    IOT_EVENT_CONNECTED,
    IOT_EVENT_DISCONNECTED,
    IOT_EVENT_COMMAND,
}SV_IOT_EVENT;

typedef enum {
    IOT_CMD_A = 0,
    IOT_CMD_B,
    IOT_CMD_NUM,
}SV_IOT_CMD;

/// EVENT CALLBACK
typedef struct {
    void 	(*on_conn)(void *arg);
    void 	(*on_disc)(void *arg);
    void 	(*on_cmd)(uint8_t cmd, void *data, void *arg);
    void 	(*on_sync)(char *data_handle,void *arg);
}sv_iot_callback_t;

/// STATE
typedef enum {
    IOT_STATE_IDLE,
    IOT_STATE_OPEN,
    IOT_STATE_CONNECT,
    IOT_STATE_SUBSCRIBE,
    IOT_STATE_RUNNING,
    IOT_STATE_DISCONNECT,
}SV_IOT_STATE;

typedef struct {
    mqtt_client_t       *mqtt_client;
    char				*device_name;
    SV_IOT_STATE 		state;
    struct {
        const char* 	topic;
        void 			(*topic_handle)(const char* data, void* arg);
    }sub_topics[IOT_SUB_TOPIC_NUMBER];
    uint8_t sub_topic_num;

    struct{
        char 			*topic;
        char 			*payload;
    }pub_topics[IOT_PUB_QUEUE_SIZE];

    uint8_t 			pub_head;
    uint8_t 			pub_tail;

    elapsed_timer_t 	timeout;

    sv_iot_callback_t 	*event_cb;
    void 			  	*event_arg;

    uint8_t 			retry;
}sv_iot_t;

sv_iot_t* sv_iot_create(mqtt_client_t	    *client,
                        sv_iot_callback_t 	*event_cb,
                        void				*event_arg);
int32_t sv_iot_push_data_to_cloud(sv_iot_t*, char*, char*);
int32_t sv_iot_process(sv_iot_t* this);

#endif //SV_IOT_H
