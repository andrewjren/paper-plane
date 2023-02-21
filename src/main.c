#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "EPD_Test.h"
#include "lwip/dns.h"

#define STRMAC(s) STR(s)
#define STR(s) #s
char ssid[] = STRMAC(WIFISSID); 
char pass[] = STRMAC(WIFIPASS);

static void dns_callback(const char *host_name, const ip_addr_t *ip_addr, void * arg)
{
    if (ip_addr) {
        printf("address: %s\n", ipaddr_ntoa(ip_addr));
    }
    else {
        printf("callback failed\n");
    }
}

int main() {
    stdio_init_all();

    sleep_ms(10000);
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_USA)) {
        printf("failed to initialize\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    
    printf("attempting to connect to %s with pass %s\n", ssid, pass);

    if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("failed to connect\n");
        return 1;
    }

    ip_addr_t ip_address;
    cyw43_arch_lwip_begin();
    int err = dns_gethostbyname("www.opensky-network.org", &ip_address, dns_callback, NULL);
    cyw43_arch_lwip_end();

    if (err == ERR_OK) {
        printf("OK! somehow\n");
    }
    else {
        printf("not ok\n");
    }

    EPD_7in5_V2_test();

    return 0;
}
