cmake_minimum_required(VERSION 3.13)

# include pico-sdk (setup on Docker)
include(/opt/pico-sdk/pico_sdk_init.cmake)

project(test C CXX ASM)

set(PICO_BOARD pico_w)

# specify C/C++ versions, pico-sdk has C++ dependencies
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

pico_sdk_init()

add_subdirectory(epaper-lib)
add_subdirectory(src)

