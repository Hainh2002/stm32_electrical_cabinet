cmake_minimum_required(VERSION 3.22)

project(sdk_utils)

set(CMAKE_C_STANDARD 11)

set(UTILS_SRC
        sm_memory.h
        sm_types.h
        containers/u_fifo.c
        containers/u_fifo.h
        timing/date_time.h
)

add_library(sdk_utils STATIC ${UTILS_SRC})
