/*
 * sync_time.h
 *
 *  Created on: Nov 9, 2024
 *      Author: admin
 */

#ifndef SERVICES_SYNC_TIME_SV_SYNC_TIME_H_
#define SERVICES_SYNC_TIME_SV_SYNC_TIME_H_

typedef struct sync_time sv_sync_time_t;
struct sync_time{
	int8_t 			timezone;
	bool			is_sync;
	sm_hal_rtc_t* 	rtc_if;
};

sv_sync_time_t* sv_sync_time_create(sm_hal_rtc_t* _rtc_if, int8_t _timezone);
int32_t sv_sync_set_datetime(sv_sync_time_t* this,const sys_datetime_t * _time);
int32_t sv_sync_get_datetime(sv_sync_time_t this, sys_datetime_t*);
int32_t sv_sync_get_time(sv_sync_time_t* this, sys_time_t*);

#endif /* SERVICES_SYNC_TIME_SV_SYNC_TIME_H_ */
