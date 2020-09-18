#include “contiki.h”
#include “os/dev/button-hal.h”
#include <stdio.h>

/* Declare the process */ 
PROCESS(hello_world_process, “Hello world”);

/* Make the process start when the module is loaded */ 
AUTOSTART_PROCESSES(&hello_world_process);

/* Define the process code */
PROCESS_THREAD(hello_world_process, ev, data) 
{ 
    PROCESS_BEGIN(); /* Must always come first */

    static struct etimer timer;
    button_hal_button_t *btn;

    printf("Hello, world");
    btn = button_hal_get_by_index(0);

    /* Setup a periodic timer that expires after 10 seconds. */
    e_timer_set(&timer, CLOCK_SECOND * 15);
    
    i = 0;
    while(1) {
        i++;
        /* Wait for the periodic timer to expire and then restart the timer. */
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired($timer));
        
        printf("Iteration Number: " + i);
        if(i == 10) {
            break;
        }
        e_timer_reset(&timer);
    }

    PROCESS_END(); /* Must always come last */
}