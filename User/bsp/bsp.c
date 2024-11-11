/*
 * bsp.c
 *
 *  Created on: Nov 8, 2024
 *      Author: admin
 */
#include "bsp.h"
#include "date_time.h"
#include "main.h"

sm_hal_uart_t *g_curr_1_uart;
sm_hal_uart_t *g_curr_2_uart;
sm_hal_uart_t *g_curr_3_uart;
sm_hal_uart_t *g_debug_uart;
sm_hal_uart_t *g_sim_uart;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern RTC_HandleTypeDef hrtc;

sm_hal_uart_t* bsp_get_sim_uart(){
	return g_sim_uart;
}
sm_hal_uart_t* bsp_get_dbg_uart(){
	return g_debug_uart;
}

void bsp_get_rtc_time(sys_time_t *time){

}

void bsp_get_rtc_datetime(sys_datetime_t *time){

}

void bsp_set_rtc_time(sys_time_t *time){

}

void bsp_uart_init(){
	g_curr_1_uart 	= sm_hal_uart_init(&huart1, 115200, 0, 0, 128);
	g_curr_2_uart	= sm_hal_uart_init(&huart2, 115200, 0, 0, 128);
	g_curr_3_uart 	= sm_hal_uart_init(&huart3, 115200, 0, 0, 128);
	g_sim_uart 		= sm_hal_uart_init(&huart4, 115200, 0, 0, 1380);
	g_debug_uart 	= sm_hal_uart_init(&huart5, 115200, 0, 0, 128);
}


void bsp_init(){
	bsp_uart_init();
}

void


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	UART_HandleTypeDef* p_uart = g_curr_1_uart->m_channel;

	if(huart->Instance == p_uart->Instance){//		HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
		sm_hal_uart_rx_irq(g_curr_1_uart, g_curr_1_uart->m_buff);
		HAL_UART_Receive_IT(&huart1, & g_curr_1_uart->m_buff, 1);
	}

	p_uart = g_curr_2_uart->m_channel;
	if(huart->Instance == p_uart->Instance){//		HAL_UART_Transmit(&huart2,&rx_data,sizeof(rx_data), 100);
		sm_hal_uart_rx_irq(g_curr_2_uart, g_curr_2_uart->m_buff);
		HAL_UART_Receive_IT(&huart2, &g_curr_2_uart->m_buff, 1);
	}

	p_uart = g_curr_3_uart->m_channel;
	if(huart->Instance == p_uart->Instance){
		sm_hal_uart_rx_irq(g_curr_3_uart, g_curr_3_uart->m_buff);
		HAL_UART_Receive_IT(&huart3, &g_curr_3_uart->m_buff, 1);
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
	}
}
