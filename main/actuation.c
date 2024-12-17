#ifndef MAIN_H
#define MAIN_H

#include "main.h"

#endif

void ledActuation()
{
    printf("\n*****LED Actuation Task Started*****\n");

    while(1)
    {
        printf("In LED task\n");
        if(xSemaphore != NULL)
        {
            if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == true)
            {
                uint8_t u8LedStatus = 0;
                if(actuationCommand == 26 && u8LedStatus == 0)
                {
                    GPIO_LED_STATE = 1 << 2;     //turn on led
                    printf("LED ON\n");
                    u8LedStatus = 1;
                }

                else if(actuationCommand == 27 && u8LedStatus == 1)
                {
                    GPIO_LED_STATE = 0 << 2;     // turn of Led
                    printf("LED OFF\n");
                    u8LedStatus = 0;
                }

                xSemaphoreGive(xSemaphore);
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}