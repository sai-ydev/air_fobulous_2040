#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsec_integration.h"

#define BME68X_TASK_STACK_SIZE      500

StaticTask_t xBME68xTaskBuffer;
TaskHandle_t xBME68xTaskHandle;
StackType_t xBME68xTaskStack[BME68X_TASK_STACK_SIZE];

return_values_init ret_bme68x;
struct bme68x_dev bme_dev;

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



void init_bme68x_task(void)
{
    xBME68xTaskHandle = xTaskCreateStatic(
        BME68x_Task,
        "BME68x_Task",
        BME68X_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 8),
        xBME68xTaskStack,
        &xBME68xTaskBuffer
    );
}