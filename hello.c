#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/temperature-sensor.h"
#include "dev/leds.h"
#include "net/rime/rime.h"
#include "random.h"

static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from) {
    printf("broadcast message received from %d.%d: '%s'\n", 
           from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}

static const struct broadcast_callbacks broadcast_cb = {broadcast_recv};
static struct broadcast_conn broadcast;


PROCESS(temp_sense, "Temperature Sense");
//PROCESS(example_broadcast_process, "Broadcast example");
//AUTOSTART_PROCESSES(&temp_sense, &example_broadcast_process);
AUTOSTART_PROCESSES(&temp_sense);
static uint8_t temp_value;

PROCESS_THREAD(temp_sense, ev, data) {

	 static struct etimer et;
    
        PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

	PROCESS_BEGIN();
	printf("Testing Sensor\n");
	temp_value = 0;

	SENSORS_ACTIVATE(button_sensor);
	SENSORS_ACTIVATE(light_sensor);
        SENSORS_ACTIVATE(temperature_sensor);

	temp_value =  light_sensor.value(0);
	printf("Temperature sensor read directly %d\n",temperature_sensor.value(0));
	printf("Temperature sensor read by variable %d\n",temp_value);
	while (1) {
		PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
		printf("Checking if fire \n");
		printf("Light sensor reads value %d\n", light_sensor.value(0));
                printf("Temperature sensor reads value %d\n", temperature_sensor.value(0));
		if(temperature_sensor.value(0) >= 10){
	                printf("Fire");
			leds_toggle(LEDS_RED);
		    broadcast_open(&broadcast, 129, &broadcast_cb);
		    while(1) {
				unsigned long ticks = CLOCK_SECOND * 2 + random_rand() % (CLOCK_SECOND * 2);
				
				etimer_set(&et, ticks);
				PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
				packetbuf_copyfrom("Hello", 6);
				broadcast_send(&broadcast);
				printf("broadcast packet sent\n");
			    }
		}
	}

	PROCESS_END();
}
