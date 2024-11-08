cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 11)

project(sdk_sv_relay)

add_library(sdk_sv_relay
        sv_relay_alarm.c
        sv_relay_alarm.h
        relay_data.h
)

target_link_libraries(sdk_sv_relay sdk_common sdk_libs)