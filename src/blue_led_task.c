#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "utils.h"
#include "blue_led_task.h"

#define LED_TASK_STACK_SIZE 500
#define DELAY pdMS_TO_TICKS(250)

typedef enum
{
    LED_TOGGLE,
    LED_DELAY,
    LED_IDLE

} e_led_blinking_state;

typedef PACKED_STRUCT
{
    e_led_blinking_state current_state;
    uint32_t start;

} blue_led_state_t;

/* Task Buffer Definitions */
StaticTask_t xLEDTaskBuffer;

/* Stack Definitions*/
StackType_t xLEDTaskStack[LED_TASK_STACK_SIZE];

/* Task Handle definitions */
TaskHandle_t xLEDTaskHandle;

blue_led_state_t blue_led = {
    .current_state = LED_IDLE,
};
/**
* @brief Blue LED blinking task
* @param Function params
* @returns None
*/
void led_blinking_task(void *pvParameters)
{

    while(1)
    {
        uint32_t now = (uint32_t) xTaskGetTickCount();

        switch(blue_led.current_state)
        {
            case LED_TOGGLE:
                gpio_put(BLUE_LED, !gpio_get(BLUE_LED));
                blue_led.current_state = LED_DELAY;
                blue_led.start = now;
                break;
            
            case LED_DELAY:
                if(now < (blue_led.start + DELAY))
                {
                    break;
                }
                blue_led.current_state = LED_TOGGLE;
                break;
            
            case LED_IDLE:
                blue_led.current_state = LED_TOGGLE;
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
        (tskIDLE_PRIORITY + 4),
        xLEDTaskStack,
        &xLEDTaskBuffer
    );
}

