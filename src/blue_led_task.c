#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "blue_led_task.h"

#define LED_TASK_STACK_SIZE 500
#define DELAY pdMS_TO_TICKS(100)

typedef enum
{
    LED_TOGGLE,
    LED_DELAY,

} e_led_blinking_state;

/* Task Buffer Definitions */
StaticTask_t xLEDTaskBuffer;

/* Stack Definitions*/
StackType_t xLEDTaskStack[LED_TASK_STACK_SIZE];

/* Task Handle definitions */
TaskHandle_t xLEDTaskHandle;

e_led_blinking_state led_state = LED_TOGGLE;
uint32_t system_tick;

/**
* @brief Blue LED blinking task
* @param Function params
* @returns None
*/
void led_blinking_task(void *pvParameters)
{
    

    while(1)
    {
        switch(led_state)
        {
            case LED_TOGGLE:
                gpio_put(BLUE_LED, !gpio_get(BLUE_LED));
                led_state = LED_DELAY;
                system_tick = xTaskGetTickCount();
                break;
            
            case LED_DELAY:
                if(xTaskGetTickCount() < (system_tick + DELAY))
                {
                    break;
                }
                led_state = LED_TOGGLE;
                break;
            default:
                break;
        }
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

