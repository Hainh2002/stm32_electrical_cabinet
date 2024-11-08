//
// Created by admin on 11/8/2024.
//

#ifndef DATE_TIME_H
#define DATE_TIME_H

typedef union datetime{
    uint8_t arr_datetime[6];
    struct {
        uint8_t year;
        uint8_t mon;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
    };
} sys_datetime_t;

typedef union time{
    uint8_t arr_time[3];
    struct {
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
    };
} sys_time_t;

#endif //DATE_TIME_H
