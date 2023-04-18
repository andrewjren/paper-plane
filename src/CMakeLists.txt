cmake_minimum_required(VERSION 3.13)

project(pico-paper-plane)

add_executable(hello main.c EPD_7in5_V2_test.c ImageData.c)
target_compile_definitions(hello PUBLIC WIFISSID=${WIFISSID} WIFIPASS=${WIFIPASS})
message("SSID: ${WIFISSID}, PASS: ${WIFIPASS}")

target_include_directories(hello PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(hello 
    pico_stdlib
    epaper
    hardware_spi
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    )

pico_enable_stdio_usb(hello 1)
pico_enable_stdio_uart(hello 0)

pico_add_extra_outputs(hello)

