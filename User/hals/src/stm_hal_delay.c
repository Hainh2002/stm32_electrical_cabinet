#include "sm_hal_delay.h"
#include "sm_hal_porting.h"

extern int64_t get_tick_count();

void sm_hal_delay_ms(uint32_t _ms){
	int64_t tickstart = get_tick_count();
	uint32_t wait = _ms;
	while ((get_tick_count() - tickstart) < wait){
		// DELAY
	}
}

void sm_hal_delay_us(uint32_t _us){
	int64_t tickstart = get_tick_count();
	uint32_t wait = _us;
	while ((get_tick_count() - tickstart) < wait){
		// DELAY
	}
}
