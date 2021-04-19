#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "contiki.h" 
#include "coap-engine.h”
/* Log configuration */
#include "sys/log.h"

#define LOG_MODULE "flag" 
#define LOG_LEVEL LOG_LEVEL_APP

/* Declare and auto-start this file's process */

PROCESS(flag_process, "Contiki-NG Border Router and CoAP Server"); 
AUTOSTART_PROCESSES(&flag_process);
extern coap_resource_t res_flag;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(flag_process, ev, data){ 
    PROCESS_BEGIN();
    coap_activate_resource(&res_flag, "res_flag");
    PROCESS_END(); 
}