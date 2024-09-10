#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "blue_led_task.h"


#define LED_TASK_STACK_SIZE 500

/* Task Buffer Definitions */
StaticTask_t xLEDTaskBuffer;

/* Stack Definitions*/
StackType_t xLEDTaskStack[LED_TASK_STACK_SIZE];

/* Task Handle definitions */
TaskHandle_t xLEDTaskHandle;

/**
* @brief Blue LED blinking task
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
        vTaskDelay(250);
    }
}

void init_blue_led_task(void)
{
    xLEDTaskHandle = xTaskCreateStatic(
        led_blinking_task,
        "LED_Blinky",
        LED_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 2),
        xLEDTaskStack,
        &xLEDTaskBuffer
    );
}

