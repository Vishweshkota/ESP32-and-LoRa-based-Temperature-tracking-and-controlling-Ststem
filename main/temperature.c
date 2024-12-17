#ifndef MAIN_H
#define MAIN_H

#include "main.h"

#endif


/*
    Converting ADC value to Actual Temperature value
*/
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
    BMP280_S32_t t_fine;
    BMP280_S32_t var1, var2, T;
    var1 = ((((adc_T>>3) - ((BMP280_S32_t)dig_T1<<1))) * ((BMP280_S32_t)dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((BMP280_S32_t)dig_T1)) * ((adc_T>>4) - ((BMP280_S32_t)dig_T1))) >> 12) *
    ((BMP280_S32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}


/*
    Reads temperature value from BMP280 Sensor
*/

void getTemperature(void* parameter)
{
    printf("\n*****Temperature Task Started*****\n");
    i2c_master_dev_handle_t* tempHandle = (i2c_master_dev_handle_t*)(parameter);
    unsigned char buffer[0x10];
    int T;
    
    while(1)
    {
        buffer[0] = 0xfa;
        i2c_master_transmit(*tempHandle, buffer, 1, -1);
        i2c_master_receive(*tempHandle, buffer, 6, -1);

        T = (((int)buffer[2] & 0xf0) >> 4) | ((int)buffer[1] << 4) | ((int)buffer[0] << 12);

        if(xSemaphore != NULL)
        {
            if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == true)
            {
                actualTemperature = (float)bmp280_compensate_T_int32(T)/100.0;
                printf("Temperature %.2f\n", actualTemperature);

                xSemaphoreGive(xSemaphore);
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}