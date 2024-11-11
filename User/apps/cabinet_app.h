//
// Created by Ng Thuy Quynh on 11/6/2024.
//

#ifndef CABINET_APP_H
#define CABINET_APP_H

#include "bsp.h"
#include "sv_iot.h"
#include "sim_a76xx.h"
#include "sv_power.h"

//#include "sv_relay_alarm.h"

typedef struct cabinet_app cabinet_app_t;

struct cabinet_app {
	struct configs {
		mqtt_config_t		*mqtt_cfg;
	}m_configs;

	struct driver {
		sim_a76xx_t 		sim_driver;
	}m_drivers;

	struct modules {
		mqtt_client_t 		*mqtt_client;
	}m_modules;

	struct services {
		sv_iot_t 			*sv_iot;
//		sv_relay_alarm 		*sv_relay;
	}m_services;

	struct flags {
		uint8_t				net_flag;
		uint8_t				init_flag;
	}m_flags;
};


void cabinet_app_init();

void cabinet_app_process();

#endif //CABINET_APP_H
