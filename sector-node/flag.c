#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "contiki.h" 
#include "coap-engine.h"
#include "os/dev/leds.h"

/* Log configuration */
#include "sys/log.h"

#define LOG_MODULE "App" 
#define LOG_LEVEL LOG_LEVEL_DBG


/* Declare and auto-start this file's process */

PROCESS(flag_process, "Flag Process"); 
AUTOSTART_PROCESSES(&flag_process);

extern coap_resource_t res_flag;
extern process_event_t POST_EVENT;

static struct etimer timer;
static int yellowFlagDefaultDuration = 20;
static bool isPersistentFlag = false;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(flag_process, ev, data){

    LOG_INFO("Starting...\n");

    PROCESS_BEGIN();

    coap_activate_resource(&res_flag, "res_flag");

    LOG_INFO("registering...\n");
	coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
	coap_set_header_uri_path(request, service_registration);

    /*
	while(!registered){	
		COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);	
    }
    */

    LOG_INFO("Init leds to green...\n");

    leds_set(LEDS_NUM_TO_MASK(LEDS_RED));

    while(true) {

        if(leds_get() == 0 && !adInterimFlag) {
            etimer_set(&timer, CLOCK_SECOND*yellowFlagDefaultDuration);
        }

        PROCESS_WAIT_EVENT();

        if(ev == POST_EVENT) {
            // Here the led is set from remote by an application

        }
    }

    PROCESS_END(); 
}