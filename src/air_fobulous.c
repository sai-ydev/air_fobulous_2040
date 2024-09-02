#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "timers.h"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c1
#define I2C_SDA 18
#define I2C_SCL 19
#define BLUE_LED 25

#define LED_TASK_STACK_SIZE 25
#define PRINT_TASK_STACK_SIZE 500


StaticTask_t xLEDTaskBuffer;
StaticTask_t xPrintTaskBuffer;

StackType_t xLEDTaskStack[LED_TASK_STACK_SIZE];
StackType_t xPrintTaskStack[PRINT_TASK_STACK_SIZE];

TaskHandle_t xLEDTaskHandle;
TaskHandle_t xPrintTaskHandle;


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
        portGET_CORE_ID();
    }
}

/**
 * @brief Printing task
 * 
 * @param pvParams 
 * @returns None
 */
void print_task(void *pvParams)
{
    while(1)
    {
        printf("Current Core: %u\n", portGET_CORE_ID());
        vTaskDelay(250);
    }
}

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    
        
    gpio_init(BLUE_LED);
    gpio_set_dir(BLUE_LED, GPIO_OUT);

    xLEDTaskHandle = xTaskCreateStatic(
                        led_blinking_task,
                        "LED_Blinky",
                        LED_TASK_STACK_SIZE,
                        NULL,
                        (tskIDLE_PRIORITY + 2),
                        xLEDTaskStack,
                        &xLEDTaskBuffer
    );
    //vTaskCoreAffinitySet(xLEDTaskHandle, (1 << 0));

    xPrintTaskHandle = xTaskCreateStatic(
        print_task,
        "print_task",
        PRINT_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 2),
        xPrintTaskStack,
        &xPrintTaskBuffer
    );
    //vTaskCoreAffinitySet(xLEDTaskHandle, (1 << 1));
    vTaskStartScheduler();

    while (true) {
        
    }
}
