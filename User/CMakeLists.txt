cmake_minimum_required(VERSION 3.22)

project(prj02)

set(CMAKE_C_STANDARD 11)

include_directories(
        utils/
        utils/containers
        utils/logger
        utils/string
        utils/timing

        common
        common/mqtt
        common/at_modem
)

add_subdirectory(common)
add_subdirectory(utils)