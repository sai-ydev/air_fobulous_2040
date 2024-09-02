#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define BLUE_LED 25

#define LED_STACK_SIZE 100


StaticTask_t xLEDTaskBuffer;

StackType_t xLEDTaskStack[LED_STACK_SIZE];

TaskHandle_t xLEDTaskHandle;


/**
* @brief BLED blinking task
* @param Function params
* @returns None
*/
void led_blinking_task(void *pvParameters)
{
    while(1)
    {
        gpio_put(BLUE_LED, true);
        vTaskDelay(250);
        gpio_put(BLUE_LED, false);
        vTaskDelay(500);
    }
}

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    gpio_init(BLUE_LED);
    gpio_set_dir(BLUE_LED, GPIO_OUT);

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    xLEDTaskHandle = xTaskCreateStatic(
                        led_blinking_task,
                        "LED_Blinky",
                        LED_STACK_SIZE,
                        NULL,
                        (tskIDLE_PRIORITY + 4),
                        xLEDTaskStack,
                        &xLEDTaskBuffer
    );  
    vTaskStartScheduler();

    while (true) {
        
    }
}
