/*
 * bsp.c
 *
 *  Created on: Nov 8, 2024
 *      Author: admin
 */
#include "bsp.h"

#include "main.h"

#include "u_queue.h"

data_queue_t g_queue;

#include "sv_power.h"

sm_hal_uart_t *g_curr_1_uart;
sm_hal_uart_t *g_curr_2_uart;
sm_hal_uart_t *g_curr_3_uart;
sm_hal_uart_t *g_debug_uart;
sm_hal_uart_t *g_sim_uart;

sm_hal_uart_t* bsp_get_sim_uart(){
	return g_sim_uart;
}
sm_hal_uart_t* bsp_get_dbg_uart(){
	return g_debug_uart;
}

void bsp_uart_init(){
	g_curr_1_uart 	= sm_hal_uart_init(&huart1, 9600, 0, 0, 128);
	g_curr_2_uart	= sm_hal_uart_init(&huart2, 9600, 0, 0, 128);
	g_curr_3_uart 	= sm_hal_uart_init(&huart3, 9600, 0, 0, 128);
	g_sim_uart 		= sm_hal_uart_init(&huart4, 115200, 0, 0, 1380);
	g_debug_uart 	= sm_hal_uart_init(&huart5, 115200, 0, 0, 128);

	queue_reset(&g_queue);
}


void bsp_init(){
	bsp_uart_init();
}




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	UART_HandleTypeDef* p_uart = g_curr_1_uart->m_channel;

	if(huart->Instance == p_uart->Instance){//		HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
		sm_hal_uart_rx_irq(g_curr_1_uart, g_curr_1_uart->m_buff);
		HAL_UART_Receive_IT(&huart1, & g_curr_1_uart->m_buff, 1);
		sm_sv_power_feed_byte(0, g_curr_1_uart->m_buff);
	}

	p_uart = g_curr_2_uart->m_channel;
	if(huart->Instance == p_uart->Instance){//		HAL_UART_Transmit(&huart2,&rx_data,sizeof(rx_data), 100);
		sm_hal_uart_rx_irq(g_curr_2_uart, g_curr_2_uart->m_buff);
		HAL_UART_Receive_IT(&huart2, &g_curr_2_uart->m_buff, 1);
		sm_sv_power_feed_byte(1, g_curr_2_uart->m_buff);
	}

	p_uart = g_curr_3_uart->m_channel;
	if(huart->Instance == p_uart->Instance){
		sm_hal_uart_rx_irq(g_curr_3_uart, g_curr_3_uart->m_buff);
		HAL_UART_Receive_IT(&huart3, &g_curr_3_uart->m_buff, 1);
		sm_sv_power_feed_byte(2, g_curr_3_uart->m_buff);
	}

	p_uart = g_sim_uart->m_channel;
	if(huart->Instance == p_uart->Instance){
		sm_hal_uart_rx_irq(g_sim_uart, g_sim_uart->m_buff);
		HAL_UART_Receive_IT(&huart4, &g_sim_uart->m_buff, 1);
	}

	p_uart = g_debug_uart->m_channel;
	if(huart->Instance == p_uart->Instance){
		sm_hal_uart_rx_irq(g_debug_uart, g_debug_uart->m_buff);
		HAL_UART_Receive_IT(&huart5, & g_debug_uart->m_buff, 1);
		queue_push(&g_queue, g_debug_uart->m_buff);
	}
}
