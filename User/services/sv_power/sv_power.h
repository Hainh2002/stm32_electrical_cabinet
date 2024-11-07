/*
 * sv_power.h
 *
 *  Created on: Nov 7, 2024
 *      Author: vypa0
 */

#ifndef USER_SERVICES_SV_POWER_SV_POWER_H_
#define USER_SERVICES_SV_POWER_SV_POWER_H_
#include "stm32f1xx_hal.h"

typedef struct{
	uint16_t m_vol;    	// * 0.1V
	uint32_t m_cur;		// * 1mA
	uint32_t m_power;
	uint32_t m_energy;
	uint16_t m_freq;
	uint16_t m_power_factor;
	uint16_t m_alarm_status;
}sv_power_phase_data_t;

typedef void(*sv_power_phase_sync_data_callback)(uint8_t status, void* arg);

void sm_sv_power_init(USART_TypeDef* _p_uart_p1, USART_TypeDef* _p_uart_p2, USART_TypeDef* _p_uart_p3);

void sm_sv_power_set_sync_cb(sv_power_phase_sync_data_callback _cb, void* _arg);

void sm_sv_power_feed_byte(uint8_t _phase_index, uint8_t _byte);

sv_power_phase_data_t* sv_power_get_phase_data(uint8_t _phase_index);

void sm_power_process();


#endif /* USER_SERVICES_SV_POWER_SV_POWER_H_ */
