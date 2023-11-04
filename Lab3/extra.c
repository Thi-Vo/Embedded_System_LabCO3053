#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOSConfig.h"
#include "esp_freertos_hooks.h"
#include "esp_log.h"

static const char *TAG = "performance";

static uint64_t idle0Calls = 0;
static uint64_t idle1Calls = 0;


static bool idle_task_0()
{
	idle0Calls += 1;
	return false;
}

static bool idle_task_1()
{
	idle1Calls += 1;
	return false;
}

//cpu 0 do sth
//cpu 1 almost free all the time
// => tick0/tick1 = process
static void cal_performance(void *args)
{
	while (1)
	{
		float idle0 = idle0Calls;
		float idle1 = idle1Calls;
		idle0Calls = 0;
		idle1Calls = 0;

		int cpu0 = 100.f - idle0 / idle1 * 100.f;
//		int cpu0 = 100.f -  idle0 / MaxIdleCalls * 100.f;
//		int cpu1 = 100.f - idle1 / MaxIdleCalls * 100.f;

		ESP_LOGI(TAG,"Process at: %d %%", cpu0);
//		ESP_LOGI(TAG, "Core 0 at %d%%", cpu0);
//		ESP_LOGI(TAG, "Core 1 at %d%%", cpu1);
		//configurable delay
		vTaskDelay(pdMS_TO_TICKS(5000));
	}
	vTaskDelete(NULL);
}


//A typical task print sth, provide work for cpu
void Task1(void *pvParameter){
	int count1 = 0;
	while(1){
		count1++;
//		printf("Start Task 1\n");
		for(int i =0;i<100;i++){
			printf("Do sth!!\n");
		}
//		printf("End Task 1\n");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        if(count1 == 5){
        	vTaskDelete(NULL);
        }
	}
	vTaskDelete(NULL);
}


void app_main(void)
{
	ESP_ERROR_CHECK(esp_register_freertos_idle_hook_for_cpu(idle_task_0, 0));
	ESP_ERROR_CHECK(esp_register_freertos_idle_hook_for_cpu(idle_task_1, 1));
	xTaskCreatePinnedToCore(&Task1, "task1", 1024, NULL, 1, NULL, 0);
	xTaskCreatePinnedToCore(&cal_performance, "perform", 2048, NULL, 1, NULL, 1);
//	xTaskCreate(cal_performance, "perform", 2048, NULL, 1, NULL);
}
