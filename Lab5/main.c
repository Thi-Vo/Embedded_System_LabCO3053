#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "FreeRTOSConfig.h"
#include <stdlib.h>
#include <inttypes.h>


#define timer1_loop 10
#define timer2_loop 5


volatile uint8_t timer1 = 0;		//ahihi
volatile uint8_t timer2 = 0;		//ihaha
volatile uint8_t time_stamp = 1;


void timer_Callback(TimerHandle_t xTimer)
{
    const uint8_t *timerID = (uint8_t*) pvTimerGetTimerID(xTimer);
    if(timerID == 0){
    	++time_stamp;
    }
    else if(timerID == 1){
    	printf("Time: %d, ahihi\n", time_stamp);
    	++timer1;
    	if(timer1 >= timer1_loop){
    		if(xTimerStop(xTimer,0) == pdFAIL){
    			printf("Fail to stop timer 1\n");
    		}
    		else{
    			printf("Timer1 stopped!\n");
    		}
    	}
    }
    else if(timerID == 2){
    	printf("Time: %d, ihaha\n", time_stamp);
    	++timer2;
    	if(timer2 >= timer2_loop){
    		if(xTimerStop(xTimer,0) == pdFAIL){
    			printf("Fail to stop timer 2\n");
    		}
    		else{
    			printf("Timer2 stopped!\n");
    		}
    	}
    }
}
void app_main(void){
	TimerHandle_t xTimer_1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(2000), pdTRUE, (void*) 1, timer_Callback);
	if(xTimer_1 == NULL){
		printf("Fail to create timer 1\n");
	}
	else{
		printf("Timer1 is successfully created!\n");
	}
	TimerHandle_t xTimer_2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(3000), pdTRUE, (void*) 2, timer_Callback);
	if(xTimer_2 == NULL){
		printf("Fail to create timer 2\n");
	}
	else{
		printf("Timer2 is successfully created!\n");
	}
	TimerHandle_t xTimer_timeStamp = xTimerCreate("Time stamp", pdMS_TO_TICKS(1000), pdTRUE, (void*) 0, timer_Callback);
	if(xTimer_1 == NULL){
		printf("Fail to create time stamp\n");
	}
	else{
		printf("Time stamp is successfully created!\n");
	}

	//Start 3 timer
	if(xTimerStart(xTimer_1,0) == pdFAIL){
		printf("Fail to start timer 1\n");
	}
	if(xTimerStart(xTimer_2,0) == pdFAIL){
		printf("Fail to start timer 2\n");
	}
	if(xTimerStart(xTimer_timeStamp,0) == pdFAIL){
		printf("Fail to start time stamp\n");
	}
    vTaskDelete(NULL);


}
