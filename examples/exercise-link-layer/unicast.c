#include "contiki.h"
//Link layer communication libraries
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
//Default libraries
#include <string.h>
#include <stdio.h>
//Other libraries
#include "sys/log.h"
#include "sys/etimer.h" 
#include "net/linkaddr.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SEND_INTERVAL_UNICAST (5 * CLOCK_SECOND) //Traffic Rate unicast
#define SEND_INTERVAL_BROADCAST (5 * CLOCK_SECOND) //Traffic Rate broadcast

static int uni_period = 0;
static int bro_period = 0;

static linkaddr_t dest_addr = {{ 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }};

PROCESS(unicast_proj, "unicast-proj");

AUTOSTART_PROCESSES(&unicast_proj);

// Callback for the reception of messages
static void input_callback() {

}

PROCESS_THREAD(unicast_proj, ev, data) {
    //Creation of the message to be sent
    static struct etimer periodic_timer;
    static struct etimer broadcast_periodic_timer;
    static char uni_msg[70];
    static char bro_msg[70];
    sprintf(uni_msg, "Hello, I send you my msg at time: %d", uni_period);
    sprintf(bro_msg, "Hello, I send you my msg at time: %d", bro_period);

    PROCESS_BEGIN();

    //Initialize NullNet
    nullnet_set_input_callback(input_callback);

    if(linkaddr_node_addr.u8[0] == 2){
        nullnet_buf = (uint8_t *)bro_msg;
        etimer_set(&broadcast_periodic_timer, SEND_INTERVAL_BRO);  
    }
    if(linkaddr_node_addr.u8[0] == 3 || linkaddr_node_addr.u8[0] == 4) { 
        nullnet_buf = (uint8_t *)uni_msg;
        etimer_set(&periodic_timer, SEND_INTERVAL);
    }

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer)|| 
            etimer_expired(&broadcast_periodic_timer));
        if(etimer_expired(&periodic_timer) && (linkaddr_node_addr.u8[0] == 3 || linkaddr_node_addr.u8[0] == 4)){
            //Send Message
        }
        if(etimer_expired(&broadcast_periodic_timer) && (linkaddr_node_addr.u8[0] == 2)){
            //Send Message
        }
    }
    PROCESS_END();
}
