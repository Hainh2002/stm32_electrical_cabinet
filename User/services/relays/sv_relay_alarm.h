//
// Created by admin on 11/8/2024.
//

#ifndef RELAY_ALARM_H
#define RELAY_ALARM_H

#include <stdint.h>
#include <stdbool.h>
#include "sm_hal.h"
#include "date_time.h"
#include "sm_elapsed_timer.h"

#define RELAY_NUMBER_MAX        3
#define SV_RELAY_PERIOD			1000

typedef struct sv_relay sv_relay_t;

typedef struct relay_data   relay_data_t;

typedef void (*update_sys_time_if)(sys_time_t *);

typedef enum {
	MANUAL_MODE = 0,
	ONLY_ALARM_ON_MODE,
	ONLY_ALARM_OFF_MODE,
	BOTH_ALARM_MODE,
}RELAY_MODE;

struct relay_data {
    void (*switch_state)(uint8_t state);
    RELAY_MODE mode;
    sys_time_t time_sw_on;
    sys_time_t time_sw_off;
};

struct sv_relay {
	uint8_t 			relays_number;
	relay_data_t 		relays[RELAY_NUMBER_MAX];
	elapsed_timer_t 	timeout;
	sys_time_t			real_time;
	update_sys_time_if	sync_if;
};

sv_relay_t* sv_relay_create(uint8_t _num, update_sys_time_if _if);

int32_t sv_relay_add(sv_relay_t* this,
						uint8_t id,
						void (*switch_state)(uint8_t),
						RELAY_MODE mode);

int32_t sv_relay_remove(sv_relay_t* this, uint8_t relay_id);

int32_t sv_relay_change_mode(sv_relay_t* this,
								uint8_t relay_id,
								RELAY_MODE mode);

int32_t sv_relay_set_alarm_common(sv_relay_t* this,
									sys_time_t* time_on,
									sys_time_t* time_off);

int32_t sv_relay_set_alarm(sv_relay_t* this,
							uint8_t relay_id,
							sys_time_t* time_on,
							sys_time_t* time_off);

int32_t sv_relay_set_state(sv_relay_t* this,
							uint8_t relay_id,
							uint8_t state);

int32_t sv_relay_process(sv_relay_t* this);

#endif //RELAY_ALARM_H
