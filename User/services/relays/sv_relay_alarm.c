//
// Created by admin on 11/8/2024.
//

#include "sv_relay_alarm.h"

sv_relay_t g_sv_relay ;

sv_relay_t* sv_relay_create(uint8_t _num, update_sys_time_if _if){
	sv_relay_t* this = &g_sv_relay;
	for (int i=0; i< _num; i++){
		this->relays[i].mode = MANUAL_MODE,

		this->relays[i].time_sw_off.hour = 0;
		this->relays[i].time_sw_off.min = 0;
		this->relays[i].time_sw_off.sec = 0;

		this->relays[i].time_sw_on.hour = 0;
		this->relays[i].time_sw_on.min = 0;
		this->relays[i].time_sw_on.sec = 0;

		this->relays[i].switch_state = NULL;
	}

	elapsed_timer_resetz(&this->timeout, SV_RELAY_PERIOD);
}

int32_t sv_relay_add(sv_relay_t* this,
						uint8_t id,
						void (*switch_state)(uint8_t),
						RELAY_MODE mode){

	if (!this || id >= this->relays_number)
		return -1;

	this->relays[id].switch_state = switch_state;
	this->relays[id].mode = mode;

	return 0;
}

int32_t sv_relay_remove(sv_relay_t* this, uint8_t relay_id){
	if (!this || id >= this->relays_number)
		return -1;

	this->relays[id].switch_state = NULL;
	this->relays[id].time_sw_off.hour = 0;
	this->relays[id].time_sw_off.min = 0;
	this->relays[id].time_sw_off.sec = 0;

	this->relays[id].time_sw_on.hour = 0;
	this->relays[id].time_sw_on.min = 0;
	this->relays[id].time_sw_on.sec = 0;
	return 0;
}

int32_t sv_relay_change_mode(sv_relay_t* this,
								uint8_t _id,
								RELAY_MODE mode){
	if (!this || id >= this->relays_number)
		return -1;

	this->relays[id].mode = mode;
	return 0;
}

int32_t sv_relay_set_alarm_common(sv_relay_t* this,
									sys_time_t* time_on,
									sys_time_t* time_off){

	if (!this || id >= this->relays_number)
		return -1;

	for (int i=0; i < this->relays_number; i++){
		this->relays[i].mode = MANUAL_MODE,

		this->relays[i].time_sw_off.hour = 0;
		this->relays[i].time_sw_off.min = 0;
		this->relays[i].time_sw_off.sec = 0;

		this->relays[i].time_sw_on.hour = 0;
		this->relays[i].time_sw_on.min = 0;
		this->relays[i].time_sw_on.sec = 0;

		this->relays[i].switch_state = NULL;
	}

}

int32_t sv_relay_set_alarm(sv_relay_t* this,
							uint8_t id,
							sys_time_t* time_on,
							sys_time_t* time_off){
	if (!this || id >= this->relays_number)
		return -1;

	this->relays[id].time_sw_off.hour = time_off->hour;
	this->relays[id].time_sw_off.min = time_off->min;
	this->relays[id].time_sw_off.sec = time_off->sec;

	this->relays[id].time_sw_on.hour = time_on->hour;
	this->relays[id].time_sw_on.min = time_on->min;
	this->relays[id].time_sw_on.sec = time_on->sec;

}

int32_t sv_relay_set_state(sv_relay_t* this,
							uint8_t id,
							uint8_t state){
	if (!this || id >= this->relays_number)
		return -1;
	this->relays[id].switch_state(state);

}


int32_t sv_relay_process(sv_relay_t* this){
	if (!this) return;
	sys_time_t real_time = {0,0,0};

	if (!elapsed_timer_get_remain(&this->timeout)){
		this->sync_if(&real_time);

		for (int i=0; i<this->relays_number; i++){
			if (this->relays[i].switch_state == NULL ||
					this->relays[i].mode == MANUAL_MODE)
				continue;

			if (real_time.hour >= this->relays[id].time_sw_on.hour &&
				real_time.min >= this->relays[id].time_sw_on.min &&
				real_time.sec >= this->relays[id].time_sw_on.sec ){

				this->relays[i].switch_state(1);
			}

			if (real_time.hour >= this->relays[id].time_sw_off.hour &&
				real_time.min >= this->relays[id].time_sw_off.min &&
				real_time.sec >= this->relays[id].time_sw_off.sec){

				this->relays[i].switch_state(0);
			}
		}
		elapsed_timer_reset(&this->timeout);
	}

}
