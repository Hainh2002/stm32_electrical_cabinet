/*
 * ra_hal_adc.c
 *
 *  Created on: Oct 1, 2024
 *      Author: vuonglk
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"
typedef void sm_hal_adc_t;


sm_hal_adc_t* sm_hal_adc_init(uint8_t _num_channel){
return NULL;
}


void sm_hal_adc_deinit(sm_hal_adc_t *_this){
return ;
}


uint16_t sm_hal_adc_read(sm_hal_adc_t *_this, uint8_t _channel){
return 0;
}


int32_t sm_hal_adc_start(sm_hal_adc_t *_this){
    return 0;
}


int32_t sm_hal_adc_stop(sm_hal_adc_t *_this){
    return 0;
}

void sm_hal_adc_set_cb(sm_hal_adc_t *_this, void(*on_converted_fn_t)(uint16_t)){
    return ;
}

int32_t sm_hal_adc_open(sm_hal_adc_t *_this){
    return 0;
}
int32_t sm_hal_adc_close(sm_hal_adc_t *_this){
    return 0;
}

