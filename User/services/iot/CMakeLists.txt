cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 11)

project(sdk_sv_iot)

add_library(sdk_sv_iot
        sv_iot.c
        sv_iot.h
        sv_iot_topic.h
        sv_iot_define.h
)

target_link_libraries(sdk_sv_iot sdk_common sdk_libs)