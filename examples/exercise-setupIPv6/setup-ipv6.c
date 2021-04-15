//Default libraries
#include "contiki.h"
#include <string.h>
#include <stdio.h>
//IPv6 Libraries
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-debug.h"
//Other libraries
#include "sys/log.h"
#include "sys/etimer.h" 

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define INTERVAL 10*CLOCK_SECOND
#define DEBUG 1

static void set_global_address(){
    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 0); 
    uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr); 
    uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
}

static void print_all_addresses() {
    //Print all the IPv6 addresses
    int i;
    uint8_t state; 
    printf("IPv6 addresses: ");
    for(i = 0; i < UIP_DS6_ADDR_NB; i++){ 
        state = uip_ds6_if.addr_list[i].state; 
        if(uip_ds6_if.addr_list[i].isused && (state == ADDR_TENTATIVE||state == ADDR_PREFERRED)){ 
            uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr); 
            printf("\n");
        }
    }
}

PROCESS(ipv6_proj, "ipv6-proj");

AUTOSTART_PROCESSES(&ipv6_proj);

PROCESS_THREAD(ipv6_proj, ev, data) {
    PROCESS_BEGIN();
    static struct etimer periodic_timer;

    etimer_set(&periodic_timer, INTERVAL);
    set_global_address();
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        etimer_reset(&periodic_timer);
        print_all_addresses();
    }

    PROCESS_END();
}