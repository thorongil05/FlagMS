#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "contiki.h" 
#include "coap-engine.h”
/* Log configuration */
#include "sys/log.h"

#define LOG_MODULE "track limit" 
#define LOG_LEVEL LOG_LEVEL_APP

/* Declare and auto-start this file's process */

PROCESS(tracklimit_process, "Track Limit Process"); 
AUTOSTART_PROCESSES(&tracklimit_process);
extern coap_resource_t res_tracklimit;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tracklimit_process, ev, data){ 
    PROCESS_BEGIN(); 
    coap_activate_resource(&res_tracklimit, "res_tracklimit");
    PROCESS_END(); 
}