#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define MSSV 2012089
#define BUTTON_PIN GPIO_NUM_15


int counter = 0;
int status = 0;

void Button_is_pressed(void *pvParameter)
{
    while(1)
    {
        status = gpio_get_level(BUTTON_PIN);
        if (status == 0)
            counter++;
        else counter = 0;
        if (counter == 1)
            printf("ESP32\n");

        vTaskDelay (100 / portTICK_PERIOD_MS ) ;
    }
    vTaskDelete(NULL);
}

void printMSSV(void *pvParameter)
{
	while(1)
    {
        printf("%d\n", MSSV);
        vTaskDelay (1000 / portTICK_PERIOD_MS ) ;
    }
    vTaskDelete(NULL);
}


void app_main()
{
    esp_rom_gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN,GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN,GPIO_PULLUP_ONLY);
    xTaskCreate(&Button_is_pressed,"buttons",8000,NULL,0,NULL);
    xTaskCreate(&printMSSV,"ID",8000,NULL,1,NULL);
}
