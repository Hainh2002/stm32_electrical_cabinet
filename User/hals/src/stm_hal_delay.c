#include "sm_hal_delay.h"
#include "sm_hal_porting.h"

void sm_hal_delay_ms(uint32_t _ms){
    HAL_Delay(_ms);
}

void sm_hal_delay_us(uint32_t _us){
    HAL_Delay(_us);
}
