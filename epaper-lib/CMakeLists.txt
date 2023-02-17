cmake_minimum_required(VERSION 3.13)

project(epaper-lib)


add_library(epaper STATIC
    # Config 
    ./Config/DEV_Config.c
    ./e-Paper/EPD_7in5_V2.c # only need to bring in 7in5 V2
    ./Fonts/font8.c
    ./Fonts/font12.c
    ./Fonts/font16.c
    ./Fonts/font20.c
    ./Fonts/font24.c
    ./Fonts/font12CN.c
    ./Fonts/font24CN.c
    ./GUI/GUI_Paint.c
    )

target_include_directories(epaper PUBLIC 
    ./Config/ 
    ./GUI/ 
    ./e-Paper/
    )

target_link_libraries(epaper 
    PRIVATE
    pico_stdlib
    hardware_spi
    )
