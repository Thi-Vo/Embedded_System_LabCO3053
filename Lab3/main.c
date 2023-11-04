#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOSConfig.h"
#include <time.h>

//GO TO FreeRTOSConfig.h and manually change the value in this header file.
//We can not redefine it here
//#define configUSE_TIME_SLICING 1 -> 0: pre-emptive, non roundrobin
//                                    1: pre-emptive + round robin

//#define configUSE_PREEMPTION 1   -> 0: non-preemptive
//                                    1: preemptive

#define CRUDE_LOOP 20000



void Task1(void *pvParameter){
	int count1 = 0;
	while(1){
		printf("Start Task 1\n");
		for(int i =0;i<CRUDE_LOOP;i++){
			count1++;
		}
		printf("End Task 1\n");
//        vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}


void Task2(void *pvParameter){
	int count2 = 0;
	while(1){
		printf("Start Task 2\n");
		for(int i =0;i<CRUDE_LOOP;i++){
			count2++;
		}
		printf("End Task 2\n");
//       vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}


//Higher priority than the other two
void Task3(void *pvParameter){
	int count3 = 0;
	while(1){
		printf("Start Task 3\n");
		for(int i =0;i<CRUDE_LOOP;i++){
			count3++;
		}
		printf("End Task 3\n");
//        vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}


void app_main(void)
{
	//We have 3 case to consider:
	//			Preemption				Time-slice				Task need
	//				1						1						2
	//				1						0						3
	//				0						x						3

	//Case 1 here
    xTaskCreatePinnedToCore(&Task1, "task1", 1024, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(&Task2, "task2", 1024, NULL, 1, NULL, 1);

    //Case2 and case3, add task number3 with higher priority than others
	if(configUSE_PREEMPTION != 1 || configUSE_TIME_SLICING != 1){
		vTaskDelay(pdMS_TO_TICKS(10000));	//some delay for easier watch preemption on monitor
		xTaskCreatePinnedToCore(&Task3, "task3", 1024, NULL, 2, NULL, 1);
	}
}
