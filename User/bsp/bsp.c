/*
 * bsp.c
 *
 *  Created on: Nov 8, 2024
 *      Author: admin
 */
#include "bsp.h"
#include "sm_hal.h"

sm_hal_uart_t g_curr_1_uart;
sm_hal_uart_t g_curr_2_uart;
sm_hal_uart_t g_curr_3_uart;
sm_hal_uart_t g_debug_uart;
sm_hal_uart_t g_sim_uart;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == g_curr_1_uart.m_channel->Instance){
	HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
	HAL_UART_Receive_IT(&huart1, &rx_data, 1);
	}
	if(huart->Instance == g_curr_2_uart.m_channel->Instance){
		HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
		HAL_UART_Receive_IT(&huart1, &rx_data, 1);
		}
	if(huart->Instance == g_curr_3_uart.m_channel->Instance){
		HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
		HAL_UART_Receive_IT(&huart1, &rx_data, 1);
		}
	if(huart->Instance == g_debug_uart.m_channel->Instance){
		HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
		HAL_UART_Receive_IT(&huart1, &rx_data, 1);
		}
	if(huart->Instance == g_sim_uart.m_channel->Instance){
		HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
		HAL_UART_Receive_IT(&huart1, &rx_data, 1);
		}
}
