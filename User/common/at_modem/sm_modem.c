#include <string.h>
#include "sm_modem.h"
#include "sm_hal_delay.h"
#include "sm_logger.h"
#include "sm_memory.h"

#define TAG  "MODEM"
#define BUFFER_SIZE		(1536)

sm_modem_t* sm_modem_init(sm_hal_io_t *io, sm_hal_uart_t *driver){
	sm_modem_t* modem = mem_alloc(sizeof(sm_modem_t));
    modem->reset_pin = io;
    modemd->river = driver;
	modem->buff = NULL;
	return modem;
}

int32_t sm_modem_cmd(sm_modem_t *modem, char* cmd, char* res_ok, char* res_fail, uint32_t timeout){
	if (!modem) return -1;

	LOG_DBG(TAG, "cmd: %s ", cmd);
    sm_hal_uart_write(modem->driver,(uint8_t*)cmd,strlen(cmd));
    
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	modem->buff = buffer;

    uint32_t time = 0;  
    uint32_t index_buff = 0;
    int32_t res = 0;
    while(1){
		size_t len = sm_hal_uart_read(modem->driver,
                                      (uint8_t*) modem->buff + index_buff,
                                      256);
        if(len > 0){
			index_buff += (uint32_t) len;
			if (strstr(modem->buff, res_ok) != NULL) {
				res = 0;
				break;
			}
			if (strstr(modem->buff, res_fail) != NULL) {
				res = -2;
				break;
			}
        }
        time++;
        if(time > timeout) {
        	LOG_ERR(TAG, "timeout");
			res = -1;
            break;
        }
        sm_hal_delay_ms(1);
    }
	LOG_DBG(TAG, "res: %s len: %d time: %d",modem->buff, strlen(modem->buff), time);
    return res;
}

int32_t sm_modem_reboot(sm_modem_t *modem, uint32_t duration_ms, int logic) {
	if (!modem) return -1;
	if (logic) {
        sm_hal_io_set_value(modem->reset_pin, 0);
		sm_hal_delay_ms(duration_ms);
        sm_hal_io_set_value(modem->reset_pin, 1);
	}
	else {
        sm_hal_io_set_value(modem->reset_pin, 1);
		sm_hal_delay_ms(duration_ms);
        sm_hal_io_set_value(modem->reset_pin, 0);
	}
}

int32_t sm_modem_write(sm_modem_t *modem, char* data, int32_t len){
	if (!modem) return -1;
	int32_t err = sm_hal_uart_write(modem->driver, (uint8_t*) data,
			(uint32_t) len);
	return err;
}

int32_t sm_modem_read(sm_modem_t *modem, char* data, uint32_t len){
	if (!modem) return -1;
	int32_t length = sm_hal_uart_read(modem->driver, (uint8_t*) data, len);
	return length;
}

int32_t sm_modem_read_until_char(sm_modem_t *modem, char* _buff, char _ch, uint32_t timeout) {
	if (!modem) return -1;
	int32_t err = 0;
	int32_t index_buff = 0;
	int32_t ret = 0;
	uint32_t time = 0;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	modem->buff = buffer;
	while (1) {
		ret = sm_hal_uart_read(modem->driver, (uint8_t*)modem->buff + index_buff, 1);
		if (ret == 1) {
			if (*(modem->buff + index_buff) == _ch) {
				memcpy(_buff, modem->buff, index_buff+1);
				return index_buff+1;
			}
	        index_buff++;
		}
		time++;
		if(time > timeout) {
			LOG_ERR(TAG, "modem read until char timeout");
			err = -1;
			break;
		}
		sm_hal_delay_ms(1);
	}
	return err;
}

int32_t sm_modem_read_until_str(sm_modem_t *modem, char* _buff, char* _str, uint32_t timeout) {
	if (!modem) return -1;
    int32_t err = 0;
    uint32_t index_buff = 0;
    int32_t ret = 0;
    uint32_t time = 0;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	modem->buff = buffer;
    while (1) {
        ret = sm_hal_uart_read(modem->driver, (uint8_t*) modem->buff + index_buff, 1);
        if (ret == 1) {
            if (strstr(modem->buff, _str)) {
                memcpy(_buff, modem->buff, index_buff+1);
                modem->lock = 0;
                return index_buff+1;
            }
            index_buff++;
        }
        time++;
        if(time > timeout) {
            LOG_ERR(TAG, "modem read until string timeout");
            err = -1;
            break;
        }
        sm_hal_delay_ms(1);
    }
    return err;
}

int32_t sm_modem_clear_buffer(sm_modem_t *modem){
	if (!modem) return -1;
	char tmp[256];
	int32_t ret = 0;
	uint32_t time = 0;
	while (1) {
		ret = sm_hal_uart_read(modem->driver, tmp, 256);
		if (ret == 0){
			break;
		}
		else{
			if (++time == 10) break;
		}
		sm_hal_delay_ms(1);
	}
}
