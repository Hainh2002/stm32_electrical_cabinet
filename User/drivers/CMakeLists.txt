cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 11)

project(sdk_drivers)

add_library(sdk_drivers
        lte/sim_a76xx.c
        lte/sim_a76xx.h
)

target_link_libraries(sdk_drivers sdk_utils)