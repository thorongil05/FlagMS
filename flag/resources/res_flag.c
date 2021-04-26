#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "coap-engine.h"
#include "coap-observe.h"
#include "os/dev/leds.h"
#include "sys/etimer.h"

#include "sys/log.h"

#define LOG_MODULE "Flag Actuator"
#define LOG_LEVEL LOG_LEVEL_DBG

static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_post_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_event_handler(void);

int actual_flag = 0;

EVENT_RESOURCE(res_flag,
	"title=\"Flag Actuator POST flag=<color>&seconds=<time> n\";methods=\"GET, POST \";rt=\"int\";obs\n",
	res_get_handler,
	res_post_handler,
    NULL,
	NULL,
	res_event_handler);

static void res_event_handler(void){
	coap_notify_observers(&res_flag);
}

static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){


	if(request != NULL){
		//LOG_DBG("Observing handler number %d\n", counter); 
	}


	unsigned int accept = -1;
	coap_get_header_accept(request, &accept);


	if (accept== -1)
		accept = APPLICATION_JSON;

	if(accept == APPLICATION_XML) {
		coap_set_header_content_format(response, APPLICATION_XML);
 		snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "<actual_flag=\"%d\"/>", actual_flag);
		coap_set_payload(response, buffer, strlen((char *)buffer));
    	} 
	else if(accept == APPLICATION_JSON) {
		coap_set_header_content_format(response, APPLICATION_JSON);
		snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"actual_flag\":%d}", actual_flag);
		coap_set_payload(response, buffer, strlen((char *)buffer));
	}
	else {
		coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
		const char *msg = "Supporting content-type application/json";
		coap_set_payload(response, msg, strlen(msg));
  	}

}

static void res_post_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {

	if(request!=NULL){
		LOG_DBG("Received POST\n");
	}

	size_t len = 0;

	const char* flag = NULL;
	const char* seconds = NULL; 

	int post_variable_flag_res = coap_get_post_variable(request, "flag", &flag);
	int post_variable_seconds_res = coap_get_post_variable(request, "seconds", &seconds);

	LOG_DBG("Flag variable read: %d\n", post_variable_flag_res);
	LOG_DBG("Seconds variable read: %d\n", post_variable_seconds_res);
	
	if(post_variable_flag_res) {
		LOG_DBG("Flag variable %s\n", flag);
	}
	if(post_variable_seconds_res) {
		LOG_DBG("Seconds variable %s\n", seconds);
	}

	if(post_variable_flag_res && post_variable_seconds_res) {
		
		if(strncmp(flag, "green", len) == 0) {
			LOG_INFO("The new flag is green\n");
			leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));
		}
		if(strncmp(flag, "yellow", len) == 0) {
			LOG_INFO("The new flag is yellow\n");
			leds_set(LEDS_NUM_TO_MASK(LEDS_YELLOW));
		}
		if(strncmp(flag, "red", len) == 0) {
			LOG_INFO("The new flag is red\n");
			leds_set(LEDS_NUM_TO_MASK(LEDS_RED));
		}
		coap_set_status_code(response, CHANGED_2_04);
	}

}
