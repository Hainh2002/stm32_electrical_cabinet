//
// Created by Ng Thuy Quynh on 11/6/2024.
//

#include "cabinet_app.h"
#include <string.h>
#include "sm_logger.h"

static char* TAG = "CABINET_APP";

void log_print(char* str){
	uint32_t len = strlen(str);
	str[len] = '\r';
	str[len+1] = '\n';
	str[len+2] = '\0';
//	sm_hal_uart_write(bsp_get_dbg_uart(), str, len+2);
}


char CLIENT_ID[32];
char HOST[128];

mqtt_config_t g_mqtt_cfg = {
		.client_id = CLIENT_ID,
		.host = HOST,
		.port = 1883,
		.user = NULL,
		.pass = NULL
};

static cabinet_app_t g_cab_app = {
		.m_configs = {
			.mqtt_cfg = &g_mqtt_cfg,
		},

		.m_modules = {
			.mqtt_client = NULL,
		},

		.m_services = {
			.sv_iot = NULL,
		},
		.m_flags = {
			.net_flag = 0,
			.init_flag = 1,
		},
};

static void 	on_iot_conn	(void *arg);
static void 	on_iot_disc	(void *arg);
static void 	on_iot_cmd	(uint8_t cmd, void *data, void *arg);
static void 	on_iot_sync	(char *data_handle,void *arg);

sv_iot_callback_t g_sv_iot_callback ={
		.on_cmd = on_iot_cmd,
		.on_conn = on_iot_conn,
		.on_disc = on_iot_disc,
		.on_sync = on_iot_sync,
};

void cabinet_app_init(){
	cabinet_app_t* this = (cabinet_app_t*)&g_cab_app;
	// BSP INIT
	bsp_init();
	sm_logger_init(log_print, LOG_LEVEL_DEBUG);
	sm_sv_power_init(&huart1, &huart2, &huart3);
	// LOAD CONFIG

	// DRIVER
	sim_a76xx_init(&this->m_drivers.sim_driver, bsp_get_sim_uart(), NULL);

	// INTERFACE
	memset(CLIENT_ID, 0, 32);
	memcpy(CLIENT_ID, "LEKhacVuong", strlen("LEKhacVuong"));

	memset(HOST, 0, 128);
	memcpy(HOST, "tcp://test.mosquitto.org", strlen("tcp://test.mosquitto.org"));

	this->m_modules.mqtt_client = mqtt_client_init(&this->m_drivers.sim_driver, this->m_configs.mqtt_cfg);
	if (!this->m_modules.mqtt_client){
		LOG_ERR(TAG, "Can't init mqtt client");
		this->m_flags.init_flag = 0;
	}
	// SERVICE
	this->m_services.sv_iot	= sv_iot_create(this->m_modules.mqtt_client, &g_sv_iot_callback, this);
	if (!this->m_services.sv_iot){
		LOG_ERR(TAG, "Can't init iot service");
		this->m_flags.init_flag = 0;
	}

	if (this->m_flags.init_flag){
		LOG_INF(TAG, "MAIN APP START");
	}else{
		LOG_WRN(TAG, "MAIN APP INIT FAIL");
	}
}

void cabinet_app_process(){
	cabinet_app_t* this = (cabinet_app_t*)&g_cab_app;
	while (1){
//		sv_iot_process(this->m_services.sv_iot);
		sm_sv_power_process();
	}
}

static void 	on_iot_conn	(void *arg){
	LOG_INF(TAG, "on_iot_conn");
	cabinet_app_t* this = (cabinet_app_t*)arg;
	this->m_flags.net_flag = 1;
}
static void 	on_iot_disc	(void *arg){
	LOG_INF(TAG, "on_iot_disc");
	cabinet_app_t* this = (cabinet_app_t*)arg;
	this->m_flags.net_flag = 0;
}
static void 	on_iot_cmd	(uint8_t cmd, void *data, void *arg){
	LOG_INF(TAG, "on_iot_cmd");
	cabinet_app_t* this = (cabinet_app_t*)arg;

}
static void 	on_iot_sync	(char *data_handle,void *arg){
	LOG_INF(TAG, "on_iot_sync");
	cabinet_app_t* this = (cabinet_app_t*)arg;
	char buff[1024];
	memset(buff, 0, 1024);
	sprintf(buff, "This is update data of %s", this->m_services.sv_iot->device_name);
	memcpy(data_handle, buff, strlen(buff));
}



