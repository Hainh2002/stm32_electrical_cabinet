cmake_minimum_required(VERSION 3.22)

project(sdk_common)

set(CMAKE_C_STANDARD 11)

add_library(sdk_common
        mqtt/mqtt_client.c
        mqtt/mqtt_client.h
)

target_link_libraries(sdk_common sdk_utils)