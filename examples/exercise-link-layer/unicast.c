#include "contiki.h"
//Link layer communication libraries
#include "os/net/nullnet/nullnet.h"
#include "os/net/netstack.h"
#include "net/linkaddr.h"
//Default libraries
#include <string.h>
#include <stdio.h>
//Other libraries
#include "sys/log.h"
#include "sys/etimer.h" 


#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SEND_INTERVAL_UNICAST (5 * CLOCK_SECOND) //Traffic Rate unicast
#define SEND_INTERVAL_BROADCAST (13 * CLOCK_SECOND) //Traffic Rate broadcast

static int uni_period = 0;
static int bro_period = 0;

static linkaddr_t dest_addr = {{ 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }};

PROCESS(unicast_proj, "unicast-proj");

AUTOSTART_PROCESSES(&unicast_proj);

// Callback for the reception of messages
static void input_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest) {
    char received_data[strlen((char *)data) + 1];
    
    if(len == strlen((char*) data) + 1) {
        memcpy(&received_data, data, strlen((char *)data) + 1);
        //Log
        LOG_INFO("TIMESTAMP: %lu, Received \"%s\", from ", clock_seconds(), received_data); 
        LOG_INFO_LLADDR(src);
        LOG_INFO_("\n");
    }
}

PROCESS_THREAD(unicast_proj, ev, data) {
    //Creation of the message to be sent
    static struct etimer periodic_timer;
    static struct etimer broadcast_periodic_timer;
    static char uni_msg[70];
    static char bro_msg[70];
    sprintf(uni_msg, "Hello, I send you my msg at time: %d", uni_period);
    sprintf(bro_msg, "Hello,  I am an annoying spammer: %d", bro_period);

    PROCESS_BEGIN();

    //Initialize NullNet
    nullnet_set_input_callback(input_callback);

    if(linkaddr_node_addr.u8[0] == 2){
        nullnet_buf = (uint8_t *)bro_msg;
        etimer_set(&broadcast_periodic_timer, SEND_INTERVAL_BROADCAST);  
    }
    if(linkaddr_node_addr.u8[0] == 3 || linkaddr_node_addr.u8[0] == 4) { 
        nullnet_buf = (uint8_t *)uni_msg;
        etimer_set(&periodic_timer, SEND_INTERVAL_UNICAST);
    }

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer)|| 
            etimer_expired(&broadcast_periodic_timer));
        if(etimer_expired(&periodic_timer) && (linkaddr_node_addr.u8[0] == 3 || linkaddr_node_addr.u8[0] == 4)){
            //Log
            LOG_INFO("TIMESTAMP: %lu, Sending UNICAST \"%s\", to ", clock_seconds(), uni_msg); 
            LOG_INFO_LLADDR(&dest_addr);
            LOG_INFO_("\n");

            //Send Message
            nullnet_len = strlen(uni_msg)+1;
            NETSTACK_NETWORK.output(&dest_addr);
            uni_period +=5;
            etimer_reset(&periodic_timer);
        }
        if(etimer_expired(&broadcast_periodic_timer) && (linkaddr_node_addr.u8[0] == 2)){
            //Log
            LOG_INFO("TIMESTAMP: %lu, Sending Multicast \"%s\", to ", clock_seconds(), bro_msg); 
            LOG_INFO_LLADDR(NULL);
            LOG_INFO_("\n");

            //Send Message
            nullnet_len = strlen(uni_msg)+1;
            NETSTACK_NETWORK.output(NULL);
            bro_period+=13;
            etimer_reset(&broadcast_periodic_timer);
        }
    }
    PROCESS_END();
}
