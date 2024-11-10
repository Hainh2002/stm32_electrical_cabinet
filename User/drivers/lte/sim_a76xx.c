#include "sim_a76xx.h"
#include "u_string_ext.h"

static int32_t reset			(sim_a76xx_t* this);
static int32_t reinit			(sim_a76xx_t* this);
static int32_t network_open		(sim_a76xx_t* this);
static int32_t mqtt_open		(sim_a76xx_t* this, const char* client_id);
static int32_t mqtt_close		(sim_a76xx_t* this);
static int32_t mqtt_connect		(sim_a76xx_t* this, const char* host, uint32_t port, const char* user,const char* pass);
static int32_t mqtt_disconnect	(sim_a76xx_t* this);
static int32_t mqtt_subscribe	(sim_a76xx_t* this, const char* topic);
static int32_t mqtt_publish		(sim_a76xx_t* this, const char* topic, const char* payload);

static int32_t ping				(sim_a76xx_t* this);
static int32_t read_sim_status	(sim_a76xx_t* this);
static int32_t set_function		(sim_a76xx_t* this);
static int32_t read_net_qual	(sim_a76xx_t* this);
static int32_t reg_net			(sim_a76xx_t* this);
static int32_t active_pdp		(sim_a76xx_t* this);

int32_t sim_a76xx_init(sim_a76xx_t* this, sm_hal_uart_t* interface, sm_hal_io_t* rst_pin){
	this->modem = sm_modem_init(rst_pin, interface);
	if (!this->modem){
		return -1;
	}
	sm_modem_reboot(this->modem, 500, 1);

	this->reg_network = 0;
	this->sim_connected = 0;

	// func
	this->reset 			= reset;
	this->reinit			= reinit;
	this->network_open 		= network_open;
	this->mqtt_open			= mqtt_open;
	this->mqtt_close		= mqtt_close;
	this->mqtt_connect      = mqtt_connect;
	this->mqtt_disconnect	= mqtt_disconnect;
	this->mqtt_subscribe	= mqtt_subscribe;
	this->mqtt_publish		= mqtt_publish;

	sm_hal_delay_ms(7000);
	return this->network_open(this);
}

int32_t sim_a76xx_proc(sim_a76xx_t* this){
	// polling read buffer
	sm_modem_t *modem = this->modem;
	char buffer[1024];
	memset(buffer, 0 ,1024);
	if (sm_modem_read(modem, buffer, 1024) >0){
		if (strstr(buffer, "+CMQTTCONNLOST:")){
			this->event_fn->on_mqtt_disconnected(this->event_arg);
			return -1;
		}
		if (strstr(buffer, "+CMQTTRXSTART:")){
			char info_buff[32];
			char topic_buff[128];
			char data_buff[1024];
			char* topic_start = NULL;
			char* data_start = NULL;
			uint8_t cid;
			uint16_t topic_len = 0;
			uint16_t data_len = 0;
			if (shortest_substring(buffer,
					"+CMQTTRXTOPIC:", "\r\n", info_buff, 32)){

				char* token = strtok(info_buff," ");
				token = strtok(NULL,",");
				token = strtok(NULL,"\r");
				topic_len = atoi(token);

				topic_start = strstr(buffer,"+CMQTTRXTOPIC:");
				topic_start = strstr(topic_start,"\r\n\r\n");
				if (!topic_start || !topic_len) {
					return 0;
				}

				topic_start += 2;
				memcpy(topic_buff, topic_start, topic_len);
			}

			memset(info_buff, 0 ,32);
			if (shortest_substring(buffer,
					"+CMQTTRXPAYLOAD:", "\r\n", info_buff, 32)){
				char* token = strtok(info_buff," ");
				token = strtok(NULL,",");
				token = strtok(NULL,"\r");
				data_len = atoi(token);

				data_start = strstr(buffer, "+CMQTTRXPAYLOAD:");
				data_start = strstr(data_start,"\r\n\r\n");
				if (!data_start || !data_len) {
					return 0;
				}

				data_start += 2;
				memcpy(data_buff, data_start, data_len);
			}

			this->event_fn->on_mqtt_recieve_msg(topic_buff, data_buff, this->event_arg);
		}
	}
	return 0;
}

int32_t sim_a76xx_reg_event(sim_a76xx_t* this, sim_a76xx_event_fn_t *fn, void *arg){
	this->event_fn = fn;
	this->event_arg = arg;
}

static int32_t reset(sim_a76xx_t* this){

}
static int32_t reinit(sim_a76xx_t* this){

}

static int32_t ping(sim_a76xx_t* this){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "ATE0\r\n");
	int32_t err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 1000);
	return err;
}
static int32_t read_sim_status(sim_a76xx_t* this){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CPIN?\r\n");
	int32_t err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 1000);
	return err;
}
static int32_t set_function		(sim_a76xx_t* this){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CFUN=1\r\n");
	int32_t err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 1000);
	return err;
}
static int32_t read_net_qual(sim_a76xx_t* this){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CSQ\r\n");
	int32_t err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 1000);
	return err;
}
static int32_t reg_net(sim_a76xx_t* this){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CREG=1\r\n");
	int32_t err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 1000);
	return err;
}
static int32_t active_pdp(sim_a76xx_t* this){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CGACT=1,1\r\n");
	int32_t err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 1000);
	return err;
}

static int32_t network_open(sim_a76xx_t* this){
	int32_t err = 0;

	err = ping				(this);
	if (err) return err;
	err = read_sim_status	(this);
	if (err) return err;
	err = set_function		(this);
	if (err) return err;
	err = read_net_qual		(this);
	if (err) return err;
	err = reg_net			(this);
	if (err) return err;
	err = active_pdp		(this);
	return err<0 ? -1 : 0;
}



static int32_t mqtt_open(sim_a76xx_t* this,
                    const char* client_id){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTSTART\r\n");
	int32_t err = sm_modem_cmd(modem, buff, "+CMQTTSTART: 0\r\n", "ERROR", 1000);

	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTACCQ=0,\"%s\"\r\n", client_id);
	err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 5000);

	return err;
}

static int32_t mqtt_close(sim_a76xx_t* this){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTREL=0\r\n");
	sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 5000);

	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTSTOP\r\n");
	sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 5000);

	return 0;
}

static int32_t mqtt_connect(sim_a76xx_t* this,
                        const char* host,
						uint32_t 	port,
                        const char* user,
                        const char* pass){
	//AT+CMQTTACCQ=0,”client test0”
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);

	if (!user || !pass){
		sprintf(buff, "AT+CMQTTCONNECT=0,\"%s:%d\",120,1\r\n", host, port);
	}else{
		sprintf(buff, "AT+CMQTTCONNECT=0,\"%s:%d\",\"%s\",\"%s\"\r\n", host, port, user, pass);
	}

	int32_t err = sm_modem_cmd(modem, buff, "+CMQTTCONNECT: 0,0", "ERROR", 5000);
	return err;
}

static int32_t mqtt_disconnect(sim_a76xx_t* this){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTDISC=0,120\r\n");
	int32_t err = sm_modem_cmd(modem, buff, "+CMQTTDISC: 0,0", "ERROR", 5000);
	return err;
}

static int32_t mqtt_subscribe(sim_a76xx_t* this, const char* topic){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTSUB=0,%d,0\r\n", strlen(topic));
	int32_t err = sm_modem_cmd(modem, buff, ">", "ERROR", 1000);

	if (err) return err;

	memset(buff, 0, 128);
	sprintf(buff, "%s", topic);
	err = sm_modem_cmd(modem, buff, "+CMQTTSUB: 0,0", "ERROR", 5000);

//	memset(buff, 0, 128);
//	sprintf(buff, "AT+CMQTTSUB=0,1");
//	err = sm_modem_cmd(modem, buff, "+CMQTTSUB: 0,0", "ERROR", 5000);
	return err;
}

static int32_t mqtt_publish(sim_a76xx_t* this, const char* topic, const char* payload){
	sm_modem_t *modem = this->modem;
	char buff[128];
	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTTOPIC=0,%d\r\n", strlen(topic));
	int32_t err = sm_modem_cmd(modem, buff, ">", "ERROR", 1000);

	if (err) return err;

	memset(buff, 0, 128);
	sprintf(buff, "%s", topic);
	err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 1000);

	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTPAYLOAD=0,%d\r\n", strlen(payload));
	err = sm_modem_cmd(modem, buff, ">", "ERROR", 1000);

	if (err) return err;

	memset(buff, 0, 128);
	sprintf(buff, "%s", payload);
	err = sm_modem_cmd(modem, buff, "OK\r\n", "ERROR", 1000);


	if (err) return err;

	memset(buff, 0, 128);
	sprintf(buff, "AT+CMQTTPUB=0,0,60\r\n");
	err = sm_modem_cmd(modem, buff, "+CMQTTPUB: 0,0", "ERROR", 10000);

	return err;
}
