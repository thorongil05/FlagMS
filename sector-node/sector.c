#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "contiki.h" 
#include "coap-engine.h"
#include "os/dev/leds.h"

/* Log configuration */
#include "sys/log.h"

#define LOG_MODULE "main" 
#define LOG_LEVEL LOG_LEVEL_DBG


/* Declare and auto-start this file's process */

PROCESS(flag_process, "Flag Process"); 
AUTOSTART_PROCESSES(&flag_process);

extern coap_resource_t res_flag;
extern coap_resource_t res_tracklimit;
extern process_event_t POST_EVENT;

extern bool trackLimitCrossed;

static struct etimer sensorTimer;

// static int yellowFlagDefaultDuration = 20;
// static bool isPersistentFlag = false;

// This function simulates a crossing of the track with a 10% of chance
bool isCrossed() {
    int p = 1 + rand()%100;
    LOG_DBG("Random Value p: " + p);
    return p <= 10;
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(flag_process, ev, data){

    LOG_INFO("Starting...\n");

    static coap_endpoint_t server_ep;
	static coap_message_t request[1];

    PROCESS_BEGIN();

    coap_activate_resource(&res_flag, "res_flag");
    coap_activate_resource(&res_tracklimit, "res_tracklimit");

    LOG_INFO("registering...\n");
	//Register the resource
    // Populate the coap_endpoint_t data structure
    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep); 
    // Prepare the message
	coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
	coap_set_header_uri_path(request, "registration");
    // Set the payload (if needed)
	char msg[4];
	sprintf(msg,"%d",node_id);
	coap_set_payload(request, (uint16_t * )msg, sizeof(msg)-1);

    /*
	while(!registered){	
		COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);	
    }
    */

    LOG_INFO("Init leds to green...\n");

    leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));

    etimer_set(&sensorTimer,10 * CLOCK_SECOND);

    while(true) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&sensorTimer));
        trackLimitCrossed = isCrossed();
        if(trackLimitCrossed) {
            LOG_INFO("A driver has crossed the limits");
            res_tracklimit.trigger();
        } else {
            LOG_INFO("No driver crossed the limit");
        }
    }

    PROCESS_END(); 
}
