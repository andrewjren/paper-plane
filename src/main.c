#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "EPD_Test.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"

#define TCP_PORT 80
#define STRMAC(s) STR(s)
#define STR(s) #s
char ssid[] = STRMAC(WIFISSID); 
char pass[] = STRMAC(WIFIPASS);

static err_t tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    printf("tcp sent callback, len: %u\n", len);
    
    return ERR_OK;
}

err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p_pbuf, err_t err) {
     printf("tcp recv callback, err: %u\n", err);
    
    return ERR_OK;
}

static err_t tcp_connect_callback(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("connect failed %d\n", err);
        return 1;
    }

    printf("connect success!\n");
    return ERR_OK;
}

static void tcp_error_callback(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        printf("tcp client error: %d\n", err);
    }
    printf("tcp error with connect\n");
}

static void dns_callback(const char *host_name, const ip_addr_t *ip_addr, void * arg)
{
    struct tcp_pcb *p_tcp_pcb = (struct tcp_pcb *) arg;
    if (ip_addr) {
        printf("address: %s\n", ipaddr_ntoa(ip_addr));

        int connect_err = tcp_connect(p_tcp_pcb, ip_addr, TCP_PORT, tcp_connect_callback);

        if (connect_err == ERR_MEM) {
            printf("memory error!\n");
        }
        else if (connect_err == ERR_OK) {
            printf("connect ok!\n");
        }
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

    struct tcp_pcb *p_tcp_pcb = calloc(1, sizeof(struct tcp_pcb));

    if (!p_tcp_pcb) {
        printf("failed calloc\n");
        return 0;
    }

    printf("registering callbacks\n");
    uint32_t dummy = 0xabcdabcd;

    tcp_arg(p_tcp_pcb, &dummy);
    tcp_sent(p_tcp_pcb, tcp_sent_callback);
    tcp_recv(p_tcp_pcb, tcp_recv_callback);
    tcp_err(p_tcp_pcb, tcp_error_callback);

    ip_addr_t ip_address;
    cyw43_arch_lwip_begin();
    int err = dns_gethostbyname("www.opensky-network.org", &ip_address, dns_callback, p_tcp_pcb);
    cyw43_arch_lwip_end();

    if (err == ERR_OK) {
        printf("OK! somehow\n");
    }
    else {
        printf("not ok\n");
        sleep_ms(5000);
    }

    cyw43_arch_lwip_begin();
    p_tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(ip_address));    
    cyw43_arch_lwip_end();
    if (!p_tcp_pcb) {
        printf("failed tcp_new\n");
        return 1;
    }
    //printf("attempting to connect\n");
    //cyw43_arch_lwip_begin();
    //int connect_err = tcp_connect(p_tcp_pcb, &ip_address, TCP_PORT, tcp_connect_callback);
    //cyw43_arch_lwip_end();

    EPD_7in5_V2_test();

    return 0;
}
