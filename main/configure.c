#ifndef MAIN_H
#define MAIN_H

#include "main.h"

#endif

/*
    GLOBAL DECLERATIONS
*/
// BMP 280 variables
unsigned short dig_T1 = 27504;
short dig_T2 = 26435;
short dig_T3 = -1000;


/*
    Configure I2C line
*/
void configureI2C(i2c_master_dev_handle_t* tempHandle)
{
    i2c_master_bus_config_t i2c_mst_config = 
    {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = SCL_PIN,
        .sda_io_num = SDA_PIN,
        .glitch_ignore_cnt = 7,
    };

    i2c_master_bus_handle_t bus_handle;
    i2c_new_master_bus(&i2c_mst_config, &bus_handle);

    i2c_device_config_t dev_cfg =
    {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x76,
        .scl_speed_hz = 100000,
    };

    i2c_master_bus_add_device(bus_handle, &dev_cfg, tempHandle);

}

/*
    Configure UART lines
*/
void configureUART()
{
    const uart_config_t uart_config = {
			.baud_rate = 115200,
			.data_bits = UART_DATA_8_BITS,
			.parity = UART_PARITY_DISABLE,
			.stop_bits = UART_STOP_BITS_1,
			.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
			.source_clk = UART_SCLK_DEFAULT,
	};

    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
	uart_param_config(UART_NUM, &uart_config);
	uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

/*
    Function to configure I2C line all BMP specific configurations 
*/
bool configureBMPSensor(i2c_master_dev_handle_t* tempHandle)
{    
    configureI2C(tempHandle);

    unsigned char buffer[0x10];

    buffer[0] = 0xf4;
    buffer[1] = 0b10110111;
    
    i2c_master_transmit(*tempHandle, buffer, 2, -1);
    
    buffer[0] = 0xd0;
    
    i2c_master_transmit(*tempHandle, buffer, 1, -1);
    i2c_master_receive(*tempHandle, buffer, 1, -1);
    
    printf("ID is %x\n", buffer[0]);
    
    buffer[0] = 0x88;
    
    i2c_master_transmit(*tempHandle, buffer, 1, -1);
    i2c_master_receive(*tempHandle, buffer, 6, -1);
    
    dig_T1 = buffer[0] | (buffer[1] << 8);
    dig_T2 = buffer[2] | (buffer[3] << 8);
    dig_T3 = buffer[4] | (buffer[5] << 8);
    
    printf("dig_T1 is %d\n", dig_T1);
    printf("dig_T2 is %d\n", dig_T2);
    printf("dig_T3 is %d\n", dig_T3);    

    return true;

}

/*
    Function to configure UART and LORA specific configurations
*/
bool configureLORA()
{
    configureUART();

    char response[BUF_SIZE];
	size_t len = command("AT+RESET", response);
	
	if (len){
		printf(response);
		len = command("AT+MODE=0", response);

		if (len){
			printf(response);
			len = command("AT+ADDRESS=10", response);

			if (len){
				printf(response);
				len = command("AT+NETWORKID=6", response);

				if (len){
					printf(response);
					len = command("AT+BAND=915000000", response);

					if (len){
						printf(response);
                        
                        return true;
					}
				}
			}
		}
	}
    return false;

}

/*
    Function to configure GPIO_2 and configurations required for LED
*/
bool configureLED()
{
    GPIO_LED_ENABLE = 1 << 2;             // GPIO_ENABLE_REG, enable GPIO2/LED
    GPIO_LED_STATE = 0 << 2;             // turn off Led

    return true;
}
/*
    Initializes all the devices connected
*/
void configureDevice(i2c_master_dev_handle_t *tempHandle)
{
    if(configureBMPSensor(tempHandle))
        printf("\n***BMP Configuration Successful***\n");
    else
        printf("\n****Failed to Initialize BMP280 Sensor*****");
    
    if(configureLORA())
        printf("\n***LoRA Configuration Successful***\n");
    else
        printf("\n****Failed to Initialize LORA module*****");
    
    if(configureLED())
        printf("\n***LED Configuration Successful***\n");
    else
        printf("\n****Failed to Initialize LED *****");
}