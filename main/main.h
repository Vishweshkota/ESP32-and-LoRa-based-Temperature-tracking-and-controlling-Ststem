/*
    This is the header file which includes all the required components(headers)
    Defines all the shared and defination variables
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "driver/uart.h"


/*
    Defining the Hardware pins
*/

//I2C PINS
#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

// LED PINS
#define GPIO_LED_ENABLE *((volatile uint32_t*)0x3FF44020)
#define GPIO_LED_STATE *((volatile uint32_t*)0x3FF44004)

//LORA
#define UART_NUM UART_NUM_2
#define TXD_PIN GPIO_NUM_2
#define RXD_PIN GPIO_NUM_15
#define BUF_SIZE 0x500

// Type def's required for BMP32 temperatur sensor
typedef int32_t BMP280_S32_t;
typedef int64_t BMP280_S64_t;  
typedef uint32_t BMP280_U32_t;
typedef uint64_t BMP280_U64_t;

/*
    Extern Variables
*/

// BMP32 variables
extern unsigned short dig_T1;
extern short dig_T2;
extern short dig_T3;

// Semaphore variable
extern SemaphoreHandle_t xSemaphore;

// Shared temperatur and actuator command
extern float actualTemperature;
extern int actuationCommand;

// Configure function to configure all the devices connected
void configureDevice(i2c_master_dev_handle_t *);

// Function to temperature value
void getTemperature(void*);

// Functions to start LoRA transmission
void loraTransmission();
size_t command(char*, char*);

// Functions to enable LED actuation
void ledActuation();
