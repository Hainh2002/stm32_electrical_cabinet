/*
 * sync_time.h
 *
 *  Created on: Nov 9, 2024
 *      Author: admin
 */

#ifndef SERVICES_SYNC_TIME_SV_SYNC_TIME_H_
#define SERVICES_SYNC_TIME_SV_SYNC_TIME_H_


#include "date_time.h"

typedef struct sync_time sv_sync_time_t;
struct sync_time{
	int8_t 			timezone;
	bool			is_sync;
	void			(*rtc_get_time)(sys_time_t*);
	void			(*rtc_update_time)(sys_time_t*);
	void 			(*ntp_get_time)(sys_time_t *);
};

sv_sync_time_t* sv_sync_time_create(, int8_t _timezone);
int32_t sv_sync_set_datetime(sv_sync_time_t* this,const sys_datetime_t * _time);
int32_t sv_sync_get_datetime(sv_sync_time_t this, sys_datetime_t*);
int32_t sv_sync_get_time(sv_sync_time_t* this, sys_time_t*);

#endif /* SERVICES_SYNC_TIME_SV_SYNC_TIME_H_ */
