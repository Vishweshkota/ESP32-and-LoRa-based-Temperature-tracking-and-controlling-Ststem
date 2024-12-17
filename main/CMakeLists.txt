idf_component_register(SRCS "main.c"
                            "configure.c"
                            "temperature.c"
                            "lora.c"
                            "actuation.c"
                    PRIV_REQUIRES spi_flash 
                    esp_driver_gpio
                    driver
                    INCLUDE_DIRS "")