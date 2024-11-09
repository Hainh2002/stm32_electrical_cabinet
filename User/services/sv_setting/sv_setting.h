/*
 * sv_setting.h
 *
 *  Created on: Nov 9, 2024
 *      Author: vuonglk
 */

#ifndef SERVICES_SV_SETTING_SV_SETTING_H_
#define SERVICES_SV_SETTING_SV_SETTING_H_

#include "stm32f1xx_hal.h"

typedef struct{
	char m_sn[32];
	uint32_t m_cur_thread_hold;
	uint32_t m_sync_time;
	uint32_t m_start_time;
	uint32_t m_stop_time;
	uint8_t m_auto;
}setting_info_t;

typedef void(*sv_setting_new_setting_cb)(setting_info_t* info, void* arg);

void sv_setting_init(USART_TypeDef* _port);

setting_info_t sv_setting_get_info();

int sv_setting_storage_info(setting_info_t info);

int sv_setting_load_info(setting_info_t* info);

void sm_sv_setting_feed_byte(uint8_t _byte);

void sv_setting_set_new_setting_cb(sv_setting_new_setting_cb _cb, void* _arg);

void sv_setting_process();

#endif /* SERVICES_SV_SETTING_SV_SETTING_H_ */
