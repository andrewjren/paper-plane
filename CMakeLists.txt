cmake_minimum_required(VERSION 3.13)

# include pico-sdk (setup on Docker)
include(/opt/pico-sdk/pico_sdk_init.cmake)

project(test C CXX ASM)

# specify C/C++ versions, pico-sdk has C++ dependencies
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

pico_sdk_init()

add_subdirectory(epaper-lib)

add_executable(hello main.c EPD_7in5_V2_test.c ImageData.c)

target_link_libraries(hello 
    pico_stdlib
    epaper
    hardware_spi
    pico_cyw43_arch_lwip_threadsafe_background
    )
pico_enable_stdio_usb(hello 1)
pico_enable_stdio_uart(hello 0)

pico_add_extra_outputs(hello)
