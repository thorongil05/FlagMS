#include "contiki.h"
#include "os/dev/button-hal.h"
#include "os/dev/leds.h"
#include "os/dev/serial-line.h"
#include <stdio.h>


PROCESS(led_project_process, "Led Project");

/* Make the process start when the module is loaded */ 
AUTOSTART_PROCESSES(&led_project_process);

PROCESS_THREAD(led_project_process, ev, data)
{
    PROCESS_BEGIN();

    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message);
        char* input = (char*)data;
        printf("received: %s\n", input);
        if(strcmp(input, "green") == 0) {
            leds_single_on(LEDS_GREEN);
        }
        if(strcmp(input, "red") == 0) {
            leds_single_on(LEDS_RED);
        }
        if(strcmp(input, "yellow") == 0) {
            leds_single_on(LEDS_YELLOW);
        }
        if(strcmp(input, "all") == 0) {
            leds_on(LEDS_ALL);
        }
        if(strcmp(input, "shut") == 0) {
            leds_off(LEDS_ALL);
        }
    }
    

    PROCESS_END();
}