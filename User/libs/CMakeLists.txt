cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 11)

project(sdk_libs)

add_library(sdk_libs
        json-maker/json-maker.h
        json-maker/json-maker.c
        tiny-json/tiny-json.h
        tiny-json/tiny-json.c
)