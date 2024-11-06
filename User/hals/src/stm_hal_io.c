#include <stdlib.h>
#include "sm_hal_io.h"
#include "sm_hal_porting.h"

typedef struct {
    GPIO_TypeDef* m_handle;
    uint16_t m_pin;
    uint16_t m_port;
    sm_hal_io_mode_t m_mode;
} stm_io_t;

static uint8_t  GPIO_OPEN_FLAG = 0;

sm_hal_io_t* sm_hal_io_init(uint16_t _pin){
    stm_io_t* gpio = malloc(sizeof(stm_io_t));

    gpio->m_pin = _pin;
    return (sm_hal_io_t*)gpio;
}

void sm_hal_io_deinit(sm_hal_io_t *_this){
    if (!_this) return;
    free(_this);
}

int32_t sm_hal_io_open(sm_hal_io_t *_this, sm_hal_io_mode_t _mode){
    if (!_this) {
        return -1;
    }
    int32_t err;
    if (!GPIO_OPEN_FLAG){
        GPIO_OPEN_FLAG = 1;
    }
    uint32_t cfg;
    if (_mode == SM_HAL_IO_INPUT){
    } else {
    }

    if (err) {
        return -1;
    }
    return 0;
}

int32_t sm_hal_io_close(sm_hal_io_t *_this){
    if (!_this) {
        return -1;
    }
    return 0;
}

int32_t sm_hal_io_set_value(sm_hal_io_t *_this, uint8_t _value){
    if (!_this) {
        return -1;
    }
    HAL_GPIO_WritePin(_this->m_handle,_this->pin,value);
    return 0;

}

uint8_t sm_hal_io_get_value(sm_hal_io_t *_this){
    if (!_this) {
        return 0;
    }
    uint8_t ret = HAL_GPIO_ReadPin(_this->handle,_this->pin);
    return ret;
}


