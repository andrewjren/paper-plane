#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "EPD_Test.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#include "lwip/apps/http_client.h"
#include "lwip/altcp.h"

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

static void result_callback(void *arg, httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err)
{
    printf("result callback: %u\n", httpc_result);
}

static err_t header_callback(httpc_state_t *connection, void *arg, struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
{
    printf("::header callback len: %u\n", content_len);
    char *buf = malloc(hdr->tot_len);

    pbuf_copy_partial(hdr, buf, hdr->tot_len, 0);

    printf("::header contents::\n%s\n",buf);

    return ERR_OK;
}

static err_t body_callback(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err)
{
    printf("::body callback\n");

    char *buf = malloc(p->tot_len);

    pbuf_copy_partial(p, buf, p->tot_len, 0);

    printf("::body contents::\n%s\n",buf);

    return ERR_OK;
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

    httpc_connection_t settings;
    settings.result_fn = result_callback;
    settings.headers_done_fn = header_callback; 

    cyw43_arch_lwip_begin();
    err_t err = httpc_get_file_dns(
            "https://www.opensky-network.org",
            80,
            "/api/states/all?lamin=45.8389&lomin=5.9962&lamax=47.8229&lomax=10.5226",
            &settings,
            body_callback,
            NULL,
            NULL
            );

    cyw43_arch_lwip_end();
    /*
    struct tcp_pcb *p_tcp_pcb = calloc(1, sizeof(struct tcp_pcb));

    if (!p_tcp_pcb) {
        printf("failed calloc\n");
        return 0;
    }

    //printf("registering callbacks\n");
    //uint32_t dummy = 0xabcdabcd;

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
*/
    EPD_7in5_V2_test();

    return 0;
}
