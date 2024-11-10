#include <stdlib.h>
#include "sm_hal_uart.h"
#include "sm_logger.h"

//#define MULTI_CALLBACK

#ifdef MULTI_CALLBACK
#define UART_CALLBACK_NUM_MAX   8
#endif

#define UART_RX_BUFFER  2048

sm_hal_uart_t* sm_hal_uart_init(const void* _channel, uint32_t _baud, uint8_t _stop_bit, uint8_t _data_bit, uint32_t buffer_size){
    if (!_channel){
        return NULL;
    }
    sm_hal_uart_t *uart = NULL;
    uart = malloc(sizeof(sm_hal_uart_t));
    uart->m_channel = _channel;
    uart->m_baud = _baud;
    uart->m_data_bit = _data_bit;
    uart->m_stop_bit = _stop_bit;
    if (!fifo_init(&uart->m_rx_buff, sizeof(uint8_t), buffer_size)){
    	return NULL;
    }
    HAL_UART_Receive_IT(_channel, &uart->m_buff, 1);
#ifdef MULTI_CALLBACK
#else
    uart->m_irq.m_tx_cb = NULL;
    uart->m_irq.m_tx_arg = NULL;

    uart->m_irq.m_rx_cb = NULL;
    uart->m_irq.m_rx_arg = NULL;
#endif
    return (sm_hal_uart_t*) uart;
}


void sm_hal_uart_deinit(sm_hal_uart_t *_this){
    if (!_this){
        return;
    }
    free(_this);
}


int32_t sm_hal_uart_config(sm_hal_uart_t *_this, uint32_t _baud, uint8_t _stop_bit, uint8_t _data_bit){
    if (!_this){
        return -1;
    }
    int32_t err = 0;
    _this->m_baud = _baud;
    _this->m_data_bit = _data_bit;
    _this->m_stop_bit = _stop_bit;
    return err ? -1 : 0;
}


int32_t sm_hal_uart_write(sm_hal_uart_t *_this, uint8_t *_buff, uint32_t _len){
    if (!_this){
        return -1;
    }
    int32_t err = 0;
    err = HAL_UART_Transmit(_this->m_channel, _buff, _len, 1000);
    return err ? -1 : 0;
}


int32_t sm_hal_uart_read(sm_hal_uart_t *_this, uint8_t *_buff, uint32_t _len){
    if (!_this){
        return -1;
    }
    int32_t err = 0;
    int32_t lenght = 0;
    while (lenght < _len) {
        if (fifo_dequeue(&_this->m_rx_buff, &_buff[lenght])) {
          lenght++;
        }else{
            break;
        }
    }
    return lenght;
}


void sm_hal_uart_set_rx_cb(sm_hal_uart_t *_this, sm_hal_uart_rx_irq_fn_t _cb, void* _arg){
    if (!_this){
        return;
    }
#ifdef MULTI_CALLBACK
#else
    _this->m_irq.m_rx_cb = _cb;
    _this->m_irq.m_rx_arg = _arg;
#endif
}


void sm_hal_uart_set_tx_cb(sm_hal_uart_t *_this, sm_hal_uart_tx_irq_fn_t _cb, void* _arg){
    if (!_this){
        return;
    }
#ifdef MULTI_CALLBACK
#else
#endif
}


int32_t sm_hal_uart_open(sm_hal_uart_t *_this){
    if (!_this){
        return -1;
    }
    int32_t err = 0;
    // int32_t err = R_SCI_UART_Open(_this->m_channel->p_ctrl, _this->m_channel->p_cfg);
    return err ? -1 : 0;
}


int32_t sm_hal_uart_close(sm_hal_uart_t *_this){
    if (!_this){
        return -1;
    }
    int32_t err = 0;
    // int32_t err = R_SCI_UART_Close(_this->m_channel->p_ctrl);
    return err ? -1 : 0;
}


int32_t sm_hal_uart_rx_irq(sm_hal_uart_t* _this, uint8_t _data){
    if (!_this){
        return -1;
    }
#ifdef MULTI_CALLBACK
#else
    static uint8_t temp_data;
    temp_data = _data;
    bool err = fifo_enqueue(&_this->m_rx_buff, &temp_data);
    if (_this->m_irq.m_rx_cb){
        _this->m_irq.m_rx_cb(_this, _data, _this->m_irq.m_rx_arg);
    }else{
       err = true;
    }
#endif
    return err ? -1 : 0;
}


int32_t sm_hal_uart_tx_irq(sm_hal_uart_t* _this, uint8_t _data){
    if (!_this){
        return -1;
    }
#ifdef MULTI_CALLBACK
#else
    if (_this->m_irq.m_tx_cb) {
        _this->m_irq.m_tx_cb(_this, _this->m_irq.m_tx_arg);
        return 0;
    }else{
        return -1;
    }
#endif
}





