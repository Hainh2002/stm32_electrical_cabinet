#include "sm_hal_i2c.h"
#include "sm_hal_porting.h"

typedef struct {
    void   *m_channel;
    sm_hal_i2c_irq_fn_t     m_cb;
    void                    *m_arg;
}ra_i2c_t;
static ra_i2c_t g_i2c;
#define impl(x)         ((ra_i2c_t*)(x))

sm_hal_i2c_t* sm_hal_i2c_init(void* _channel, uint8_t _addr){
    ra_i2c_t* i2c = &g_i2c;
    i2c->m_channel = _channel;
    return (sm_hal_i2c_t*)i2c;
}

void sm_hal_i2c_deinit(sm_hal_i2c_t *_this){
    if (!_this) return;
    impl(_this)->m_channel = NULL;
}

void sm_hal_i2c_set_callback(sm_hal_i2c_t *_this, sm_hal_i2c_irq_fn_t _cb, void*_arg){
    if (!_this) return;
    impl(_this)->m_cb = _cb;
    impl(_this)->m_arg = _arg;
}

int32_t sm_hal_i2c_set_addr(sm_hal_i2c_t *_this, uint8_t _addr){
    if (!_this) return -1;
}

int32_t sm_hal_i2c_open(sm_hal_i2c_t *_this){
    if (!_this) return -1;
}

int32_t sm_hal_i2c_close(sm_hal_i2c_t *_this){
    if (!_this) return -1;
}

int32_t sm_hal_i2c_write(sm_hal_i2c_t *_this, const uint8_t *_data, uint32_t _len){
    if (!_this) return -1;
}

int32_t sm_hal_i2c_read(sm_hal_i2c_t *_this, uint8_t *_data, uint32_t _len){
    if (!_this) return -1;
}

int32_t sm_hal_i2c_callback(sm_hal_i2c_t *_this, uint8_t event){
    if (!_this || !impl(_this)->m_cb) return -1;
    impl(_this)->m_cb(_this, event, impl(_this)->m_arg);
    return 0;
}

