//
// Created by Ng Thuy Quynh on 11/6/2024.
//

#include "cabinet_app.h"
#include "bsp.h"

sm_hal_uart_t *test_port;

void main_app_setup(){
	// BSP INIT
	bsp_init();

}

void main_app_loop(){
	test_port = bsp_get_dbg_uart();
	while (1){
		char buff[256];
		memset(buff, 0, 256);
		if (sm_hal_uart_read(test_port, buff, 256) > 0){
			sm_hal_uart_write(test_port, buff, strlen(buff));
		}

		sm_hal_delay_ms(1000);
	}
}
