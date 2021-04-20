#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "contiki.h" 
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "sys/node-id.h"
#include "sys/etimer.h"

/* Log configuration */
#include "sys/log.h"


#define LOG_MODULE "track limit" 
#define LOG_LEVEL LOG_LEVEL_DBG

/* Declare and auto-start this file's process */

PROCESS(tracklimit_process, "Track Limit Process"); 
AUTOSTART_PROCESSES(&tracklimit_process);

extern coap_resource_t res_tracklimit;

// Server IP and resource path
#define SERVER_EP "coap://[fd00::1]:5683"

static struct etimer timer;
bool registered = false;

extern bool trackLimitCrossed;

bool isCrossed() {
    int p = 1 + rand()%100;
    return p <= 10;
}

void client_chunk_handler(coap_message_t *response) {
    const uint8_t *chunk;
    if(response == NULL) { 
        puts("Request timed out"); 
        return;
    }
    int len = coap_get_payload(response, &chunk);
    printf("|%.*s", len, (char *)chunk); 
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tracklimit_process, ev, data){ 

    static coap_endpoint_t server_ep;
	static coap_message_t request[1];

    PROCESS_BEGIN();

    //Activation of the resource
    coap_activate_resource(&res_tracklimit, "res_tracklimit");

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
    
    //DECOMMENT
	// while(!registered){
	// 	LOG_DBG("Retrying registration..\n");
	// 	COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);
	// }
	LOG_DBG("Track Limit Sensor Registered\n");

    etimer_set(&timer,5 * CLOCK_SECOND);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        trackLimitCrossed = isCrossed();
        if(trackLimitCrossed) {
            LOG_DBG("A driver has crossed the limits");
            res_tracklimit.trigger();
        } else {
            LOG_DBG("No driver crossed the limit");
        }
        etimer_reset(&timer);
    }
    PROCESS_END(); 
}