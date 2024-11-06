/*
 * sm_modem.h
 *
 *  Created on: Jul 5, 2023
 *      Author: Admin
 */

#ifndef COMMON_MODEM_SM_MODEM_H_
#define COMMON_MODEM_SM_MODEM_H_

#include <stdbool.h>
#include "sm_hal.h"
#include "u_fifo.h"
#define MODEM_BUFF_SIZE 4096

typedef struct sm_modem sm_modem_t;

struct sm_modem{
    sm_hal_io_t *reset_pin;
    sm_hal_uart_t *driver;
    char* buff;
    volatile bool lock;
};

sm_modem_t *sm_modem_init(sm_hal_io_t *io, sm_hal_uart_t *driver);

int32_t sm_modem_cmd(sm_modem_t *modem, char* cmd, char* res_ok, char* res_fail, uint32_t timeout);

int32_t sm_modem_reboot(sm_modem_t *modem, uint32_t duration_ms, int logic);

int32_t sm_modem_write(sm_modem_t *modem, char* data, int32_t len);

int32_t sm_modem_read(sm_modem_t *modem, char* data, uint32_t len);

int32_t sm_modem_read_until_char(sm_modem_t *modem, char* data, char chr, uint32_t timeout);

int32_t sm_modem_read_until_str(sm_modem_t *modem, char* data, char* str, uint32_t timeout);

int32_t sm_modem_clear_buffer(sm_modem_t *modem);
#endif/* COMMON_MODEM_SM_MODEM_H_ */
