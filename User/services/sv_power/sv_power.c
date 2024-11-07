/*
 * sv_power.c
 *
 *  Created on: Nov 7, 2024
 *      Author: vypa0
 */

#include "sv_power.h"
#include "sm_mb_master_impl.h"
#include "u_queue.h"
#include "sm_elapsed_timer.h"
#include "sm_logger.h"

#define TAG "sv_power"

#define PHASE_NUMBER 		3
#define SYNC_TIME_PERIOD  	2000

typedef struct{
	sm_mb_master_t* m_mb_master;
	data_queue_t m_data_queue[PHASE_NUMBER];
	uint8_t m_current_phase_read;
	USART_TypeDef* m_p_uart[PHASE_NUMBER];
	sv_power_phase_data_t m_phase_data[PHASE_NUMBER];
	elapsed_timer_t m_sync_timer;

	sv_power_phase_sync_data_callback m_cb;
	void* m_cb_arg;
}sv_power_impl_t;

sv_power_impl_t g_power;

static int32_t mb_master_send_if(const uint8_t* _data, int32_t _len, int32_t _timeout, void* _arg){
	if(g_power.m_current_phase_read >= PHASE_NUMBER){
		return -1;
	}
    return 	HAL_UART_Transmit(g_power.m_p_uart[g_power.m_current_phase_read], _data, _len, _timeout);
}
static int32_t mb_master_rcv_if(uint8_t* _buf, int32_t _len, int32_t _timeout, void* _arg){
	if(g_power.m_current_phase_read >= PHASE_NUMBER){
		return -1;
	}
    return queue_get_bytes(&g_power.m_data_queue[g_power.m_current_phase_read], _buf, _len, _timeout);
}


void sm_sv_power_init(USART_TypeDef* _p_uart_p1, USART_TypeDef* _p_uart_p2, USART_TypeDef* _p_uart_p3){
	g_power.m_p_uart[0] = _p_uart_p1;
	g_power.m_p_uart[1] = _p_uart_p2;
	g_power.m_p_uart[2] = _p_uart_p3;

	g_power.m_current_phase_read = 0;

	for(int i = 0; i < PHASE_NUMBER; i++){
		queue_reset(&g_power.m_data_queue[i]);
	}
	elapsed_timer_resetz(&g_power.m_sync_timer, SYNC_TIME_PERIOD);
	g_power.m_mb_master = sm_mb_master_create(mb_master_send_if, mb_master_rcv_if, &g_power);
}

void sm_sv_power_set_sync_cb(sv_power_phase_sync_data_callback _cb, void* _arg){
	g_power.m_cb = _cb;
	g_power.m_cb_arg = _arg;
}

void sm_sv_power_feed_byte(uint8_t _phase_index, uint8_t _byte){
	if(_phase_index >= PHASE_NUMBER){
		return;
	}
	queue_push(&g_power.m_data_queue[_phase_index], _byte);
}

sv_power_phase_data_t* sv_power_get_phase_data(uint8_t _phase_index){
	if(_phase_index > PHASE_NUMBER){
		return NULL;
	}
	return &g_power.m_phase_data[_phase_index];
}

void sm_power_process(){
	if(!elapsed_timer_get_remain(&g_power.m_sync_timer)){
		if(g_power.m_current_phase_read++ >= PHASE_NUMBER){
			g_power.m_current_phase_read = 0;
		}

		uint8_t id = g_power.m_current_phase_read;

		uint16_t buffer[10] = {0,};
		int ret = sm_sv_mb_master_read_input_regs(g_power.m_mb_master, 1, 0, 10, buffer);
		if(ret == MODBUS_ERROR_NONE){
			memcpy(&g_power.m_phase_data[id], buffer, sizeof(sv_power_phase_data_t));
			LOG_INF(TAG, "Read phase %d data SUCCEED!!!");
			LOG_INF(TAG, "Phase %d current is %d", id, g_power.m_phase_data[id].m_cur);
		}else{
			LOG_ERR(TAG, "Read phase %d data FAILED, reset to 0", id);
			memset(&g_power.m_phase_data[id], 0, sizeof(sv_power_phase_data_t));
		}

		if(g_power.m_cb){
			g_power.m_cb(ret == MODBUS_ERROR_NONE, g_power.m_cb_arg);
		}

		elapsed_timer_reset(&g_power.m_sync_timer);
	}
}





