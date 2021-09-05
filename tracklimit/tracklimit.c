#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
//#include "sys/node-id.h"
#include "contiki.h" 
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "os/dev/leds.h"
#include "sys/etimer.h"

/* Log configuration */
#include "sys/log.h"
#include "coap-log.h"

#define LOG_MODULE "Tracklimit" 
#define LOG_LEVEL LOG_LEVEL_DBG

/* Declare and auto-start this file's process */

PROCESS(tracklimit_process, "Flag Process"); 
AUTOSTART_PROCESSES(&tracklimit_process);

extern coap_resource_t res_tracklimit;

extern bool trackLimitCrossed;

bool registered = false;

#define SERVER_EP "coap://[fd00::1]:5683"

void client_chunk_handler(coap_message_t *response){
  
	const uint8_t *chunk;

	if(response == NULL) {
		puts("Request timed out");
	return;
	}

	if(!registered)
	registered = true;

	int len = coap_get_payload(response, &chunk);
	printf("|%.*s", len, (char *)chunk);
}

// static int yellowFlagDefaultDuration = 20;
// static bool isPersistentFlag = false;

// This function simulates a crossing of the track with a 10% of chance
bool isCrossed() {
    int p = 1 + rand()%100;
    LOG_DBG("Random Value p: %d\n",p);
    return p <= 10;
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tracklimit_process, ev, data){

    static struct etimer sensorTimer;

    static coap_endpoint_t server_ep;
	static coap_message_t request[1];

    PROCESS_BEGIN();
	LOG_INFO("Starting...\n");	

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

	while(!registered){	
		COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);	
    }

    etimer_set(&sensorTimer,10 * CLOCK_SECOND);

    while(true) {
        PROCESS_WAIT_EVENT();
        if(ev == PROCESS_EVENT_TIMER) {
            if(etimer_expired(&sensorTimer)) {
                trackLimitCrossed = isCrossed();
                if(trackLimitCrossed) {
                    LOG_INFO("A driver has crossed the limits\n");
                    res_tracklimit.trigger();
                } else {
                    LOG_INFO("No driver crossed the limit\n");
                }
            }
        }
        etimer_reset(&sensorTimer);
    }

    PROCESS_END(); 
}
