//
// Created by admin on 11/8/2024.
//

#ifndef RELAY_ALARM_H
#define RELAY_ALARM_H

#include <stdint.h>
#include "sm_hal.h"
#include "date_time.h"

#define RELAY_NUMBER_MAX        3

typedef struct sv_relay sv_relay_t;


typedef struct relay_data   relay_data_t;

struct relay_data {
    void (*sw_on)(void*);
    void (*sw_off)(void*);
    uint8_t state;
};

struct sv_relay {
    // uint8_t relays_number;

};



#endif //RELAY_ALARM_H
