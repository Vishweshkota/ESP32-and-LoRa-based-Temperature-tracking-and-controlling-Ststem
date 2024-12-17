#ifndef MAIN_H
#define MAIN_H

#include "main.h"

#endif

// Global Variables
float actualTemperature = 0 ;
int actuationCommand = 1;
SemaphoreHandle_t xSemaphore = NULL;


/*
    Main Function
*/
void app_main()
{
    i2c_master_dev_handle_t tempHandle;

    printf("\n*****Configuring the device*****\n");
    configureDevice(&tempHandle);

    TaskHandle_t tempTask, ledTask, LORATask;

    xSemaphore = xSemaphoreCreateBinary();
    if(xSemaphore == NULL)
    {
        printf("Failed to create Semaphore");
    }

    /* Starting individual tasks for 
        - start getting temperature readouts from sensor
        - startLoRA transmimssion
        - start led actuation
    */
    xTaskCreatePinnedToCore(getTemperature, "Temperature_Task", 4096, &tempHandle, 0, &tempTask, 0);
    xTaskCreatePinnedToCore(loraTransmission, "LORA_Task", 8192, NULL, 0, &LORATask, 0);
    xTaskCreatePinnedToCore(ledActuation, "LED_Actuation_Task", 2048, NULL, 0, &ledTask, 1);

    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }

}