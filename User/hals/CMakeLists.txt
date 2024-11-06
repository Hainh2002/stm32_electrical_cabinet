cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 11)

project(sdk_hals)

add_library(sdk_hals
        include/sm_hal.h
        include/sm_hal_io.h
        include/sm_hal_delay.h
        include/sm_hal_io.h
        include/sm_hal_uart.h

        src/stm_hal_io.c
        src/stm_hal_uart.c
        src/stm_hal_delay.c
)

target_link_libraries(sdk_hals sdk_utils)