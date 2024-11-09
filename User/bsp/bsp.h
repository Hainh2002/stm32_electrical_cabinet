/*
 * bsp.h
 *
 *  Created on: Nov 8, 2024
 *      Author: admin
 */

#ifndef USER_BSP_BSP_H_
#define USER_BSP_BSP_H_

#include "sm_hal.h"

sm_hal_uart_t* bsp_get_sim_uart();

sm_hal_uart_t* bsp_get_dbg_uart();

void bsp_uart_init();


#endif /* USER_BSP_BSP_H_ */
