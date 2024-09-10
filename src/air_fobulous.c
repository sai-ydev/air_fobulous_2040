#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "task.h"
#include "timers.h"
#include "common.h"
#include "bsec_integration.h"
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

#define BME68X_TASK_STACK_SIZE      500
#define ZMOD4x10_TASK_STACK_SIZE    500

return_values_init ret_bme68x;
struct bme68x_dev bme_dev;

StaticTask_t xSDCardTaskBuffer;

StaticTask_t xBME68xTaskBuffer;
StaticTask_t xZMOD4x10TaskBuffer;

StackType_t xSDCardTaskStack[SD_CARD_TASK_STACK_SIZE];

StackType_t xBME68xTaskStack[BME68X_TASK_STACK_SIZE];
StackType_t xZMOD4x10TaskStack[ZMOD4x10_TASK_STACK_SIZE];

TaskHandle_t xSDCardTaskHandle;

TaskHandle_t xBME68xTaskHandle;
TaskHandle_t xZMOD4x10TaskHandle;

SemaphoreHandle_t xI2CSemaphore;
StaticSemaphore_t xI2CSemaphore_Buffer;

/**
 * @brief BME688 task
 * 
 * @return none
 */
void BME68x_Task(void *pvParams)
{
    
    memset(&bme_dev,0,sizeof(bme_dev));

    bme68x_interface_init(&bme_dev, BME68X_I2C_INTF);

    while(1)
    {
        printf("BME Task Core: %u\n", portGET_CORE_ID());
        vTaskDelay(250);
    }
}


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

    xBME68xTaskHandle = xTaskCreateStatic(
        BME68x_Task,
        "BME68x_Task",
        BME68X_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 8),
        xBME68xTaskStack,
        &xBME68xTaskBuffer
    );
    //vTaskCoreAffinitySet(xLEDTaskHandle, (1 << 1));
    vTaskStartScheduler();

    while (true) {
        
    }
}
