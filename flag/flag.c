#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "contiki.h" 
#include "sys/node-id.h" // for the node id
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "os/dev/leds.h"
#include "sys/etimer.h"

/* Log configuration */
#include "sys/log.h"
#include "coap-log.h"

#define LOG_MODULE "App" 
#define LOG_LEVEL LOG_LEVEL_DBG

bool registered = false;

/* Declare and auto-start this file's process */

PROCESS(temporary_yellow_flag,"Restore Green Flag after x seconds of Yellow Flag");

PROCESS(flag_process, "Flag Process"); 
AUTOSTART_PROCESSES(&flag_process, &temporary_yellow_flag);

extern int actual_flag;

extern coap_resource_t res_flag;

extern process_event_t POST_EVENT;

extern int yellowFlagDuration;

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

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(flag_process, ev, data){

	//static struct etimer yellowFlagTimer;

    static coap_endpoint_t server_ep;
	static coap_message_t request[1];

    PROCESS_BEGIN();
	LOG_INFO("Starting...\n");	


    coap_activate_resource(&res_flag, "res_flag");

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

    LOG_INFO("Init leds to green...\n");

    leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));


    while(true) {
        PROCESS_WAIT_EVENT();
    }

    PROCESS_END(); 
}

PROCESS_THREAD(temporary_yellow_flag, ev, data) {
	static struct etimer yellowFlagTimer;
	PROCESS_BEGIN();

	while (true) {
		PROCESS_WAIT_EVENT();
		if(ev == POST_EVENT) {
			LOG_INFO("Temporarly Yellow Flag is set\n");
			LOG_INFO("Duration: %d\n", yellowFlagDuration);
			etimer_set(&yellowFlagTimer, yellowFlagDuration * CLOCK_SECOND);
			actual_flag = 1;
		}
		if(etimer_expired(&yellowFlagTimer)) {
			LOG_INFO("Timer Expired: flag becomes again green\n");
			leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));
			actual_flag = 0;
		}
	}
	PROCESS_END();
}
