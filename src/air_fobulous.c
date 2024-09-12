#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "task.h"
#include "timers.h"
#include "common.h"

#include "blue_led_task.h"
#include "neopixel_task.h"
#include "print_task.h"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c1
#define I2C_SDA 18
#define I2C_SCL 19

#define SD_CARD_TASK_STACK_SIZE     500

StaticTask_t xSDCardTaskBuffer;
StackType_t xSDCardTaskStack[SD_CARD_TASK_STACK_SIZE];
TaskHandle_t xSDCardTaskHandle;

SemaphoreHandle_t xI2CSemaphore;
StaticSemaphore_t xI2CSemaphore_Buffer;



/**
 * @brief 
 * 
 * @return int 
 */
int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    
        
    gpio_init(BLUE_LED);
    gpio_set_dir(BLUE_LED, GPIO_OUT);

    xI2CSemaphore = xSemaphoreCreateMutexStatic(&xI2CSemaphore_Buffer);
    if(xI2CSemaphore == NULL)
    {
        printf("Failed to create semaphore handle\n");
        while(1);
    }

    /* Task definition in blue_led_task.c - Same goes to other tasks*/
    init_blue_led_task();

    init_neopixel_task();

    init_print_task();    

    
    //vTaskCoreAffinitySet(xLEDTaskHandle, (1 << 1));
    vTaskStartScheduler();

    while (true) {
        
    }
}
