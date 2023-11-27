#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/queue.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "driver/gpio.h"


struct Task{
	char taskName[10];
	int taskID;
};
struct QueueData{
	char taskName[10];
	int taskID;
	int timeChecked;
};

QueueHandle_t xQueue;

struct Task Read = {.taskName = "read",.taskID = 1};
struct Task Listen = {.taskName = "listen",.taskID = 2};
struct Task Walk = {.taskName = "walk",.taskID = 3};
struct Task Sing = {.taskName = "sing",.taskID = 4};


void Create_xQueue(void *pvParameter)
{
    xQueue = xQueueCreate(12, sizeof(struct QueueData *));		//queue by reference
    if (xQueue == NULL)
    {
        printf("Failed to create queue, not enough memory\n");
    }
    vTaskDelete(NULL);
}

void reception_Task(void *pvParameter)
{
    time_t t;
    srand((unsigned)time(&t));

    while(1)
    {
        while (xQueue == NULL);			//wait for queue

        int random_job = (rand() % 4) + 1;			//job 1 to 4
        int delay = ((rand() % 6) + 1)*100;			//random time 100->600ms
        struct QueueData *xData = malloc(sizeof(struct QueueData));

        if (xData == NULL)
        {
        	printf("Fail to create queue data, not enough memory");
        }
        else
        {
            switch (random_job)
            {
            case 1:
            	strcpy(xData->taskName, "read");
                xData->taskID = 1;
                xData->timeChecked = 0;
                break;
            case 2:
            	strcpy(xData->taskName, "listen");
                xData->taskID = 2;
                xData->timeChecked = 0;
                break;
            case 3:
            	strcpy(xData->taskName, "walk");
                xData->taskID = 3;
                xData->timeChecked = 0;
                break;
            case 4:
            	strcpy(xData->taskName, "sing");
                xData->taskID = 4;
                xData->timeChecked = 0;
                break;
            }

            //send xData to queue
            if (xQueueSendToBack(xQueue, (void *)&xData, 100) == errQUEUE_FULL)
            {
                printf("Failed to add job with ID %d\n", random_job);
            }
            else{
            	printf("Job with id %d join queue.\n", random_job);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(delay));
    }

    vTaskDelete(NULL);
}

void func_Task(void *pvParameter)
{
    while(1)
    {
        struct Task *task = (struct Task*)pvParameter;
        struct QueueData *currentTask;
        if (xQueue == NULL)
        {
        	printf("Empty queue\n");
        }
        else
        {
            if (xQueueReceive(xQueue, &currentTask, pdMS_TO_TICKS(100)) == pdPASS)
            {
            	printf("Task id %d meets id %d!\n", task->taskID, currentTask->taskID);
                if (currentTask->taskID == task->taskID)
                {
                    printf("Task %d is successfully executed!!\n", currentTask->taskID);
                    // always remember to free the memory when done.
                    free(currentTask);
                }
                else
                {
                    printf("%s: received %s, not accepted\n", task->taskName, currentTask->taskName);

                    if(currentTask->timeChecked < 1)		//1 = number of functional task - 1
                    {
                    	currentTask->timeChecked++;
                    	xQueueSendToFront(xQueue, (void *)&currentTask, pdMS_TO_TICKS(100));
                    }
                    else
                    {
                        printf("ERROR: Task %s with id %d is rejected, skiping this task.\n", currentTask->taskName, currentTask->taskID);
                        free(currentTask);
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(&Create_xQueue, "queue_start", 2048, NULL, 2, NULL);
    xTaskCreate(&reception_Task, "reception", 2048, NULL, 1, NULL);
    xTaskCreate(&func_Task, "read", 2048, (void *)&Read, 1, NULL);
    xTaskCreate(&func_Task, "listen", 2048, (void *)&Listen, 1, NULL);
}
