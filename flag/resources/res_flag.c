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
process_event_t POST_EVENT;


EVENT_RESOURCE(res_flag,
	"title=\"Flag Actuator POST flag=<color>&seconds=<time>\";methods=\"GET,POST \";rt=\"int\";obs\n",
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
	if(accept == TEXT_PLAIN) {
	    coap_set_header_content_format(response, TEXT_PLAIN);
	    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "actual_flag=%d", actual_flag);
	    coap_set_payload(response, (uint8_t *)buffer, strlen((char *)buffer));    
	} else if(accept == APPLICATION_XML) {
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

	size_t len_flag_parameter;
	size_t len_seconds_parameters;

	const char* flag = NULL;
	const char* seconds = NULL; 

	len_flag_parameter = coap_get_post_variable(request, "flag", &flag);
	len_seconds_parameters = coap_get_post_variable(request, "seconds", &seconds);

	if(len_flag_parameter && len_seconds_parameters) {

		if(strncmp(flag, "green", len_flag_parameter) == 0) {
			LOG_INFO("The new flag is green\n");
			leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));
			actual_flag = 0;
		}
		if(strncmp(flag, "yellow", len_flag_parameter) == 0) {
			LOG_INFO("The new flag is yellow\n");
			leds_set(LEDS_NUM_TO_MASK(LEDS_YELLOW));
			actual_flag = 1;
			process_post(&temporary_yellow_flag, POST_EVENT, NULL); //Data is pointer to void, so we can pass any type of pointer
		}
		if(strncmp(flag, "red", len_flag_parameter) == 0) {
			LOG_INFO("The new flag is red\n");
			leds_set(LEDS_NUM_TO_MASK(LEDS_RED));
			actual_flag = 2;
		}
		coap_set_status_code(response, CHANGED_2_04);
	} else {
		coap_set_status_code(response, BAD_REQUEST_4_00);
	}

}
