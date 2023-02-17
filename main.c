#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "EPD_Test.h"

char ssid[] = "NETGEAR64";
char pass[] = "cloudybanana010";

int main() {
    stdio_init_all();
    //setup_default_uart();
    //printf("Hello, world!\n");
    DEV_Delay_ms(500);

    EPD_7in5_V2_test();

    return 0;
}
