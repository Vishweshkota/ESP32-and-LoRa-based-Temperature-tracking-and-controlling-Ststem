#ifndef MAIN_H
#define MAIN_H

#include "main.h"

#endif

size_t command(char* cmd, char* response)
{
	char tmp[BUF_SIZE];
	sprintf(tmp, "%s\r\n", cmd);
	uart_write_bytes(UART_NUM, tmp, strlen(tmp));

	size_t len = uart_read_bytes(UART_NUM, response, (BUF_SIZE - 1), 100 / portTICK_PERIOD_MS);
	
	if (len)
	{
		response[len] = 0;
	}
	
	return len;
}

void loraTransmission()
{
	printf("\n*****Starting LoRA Transmission Task*****\n");
	size_t len;
	char response[BUF_SIZE];
	char msg[0x100];
	while(1)
	{
		if(xSemaphore != NULL)
    	{
			if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == true)
        	{
				sprintf(msg, "%f", actualTemperature);
			
				char tmp[BUF_SIZE];
				sprintf(tmp, "AT+SEND=11,%d,%s", strlen(msg), msg);
				
				len = command(tmp, response);

				if (len > 10)
				{
					printf("Responds received\n");
					printf(response);
					// printf("%d\n",strlen(response));
					actuationCommand = strlen(response);
					printf("Actuation command received\n");
				}

				xSemaphoreGive(xSemaphore);
			}
		}
		
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}