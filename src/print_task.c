#include "pico/stdlib.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define PRINT_TASK_STACK_SIZE  500

StaticTask_t xPrintTaskBuffer;

StackType_t xPrintTaskStack[PRINT_TASK_STACK_SIZE];

TaskHandle_t xPrintTaskHandle;

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
        
    }
}

void init_print_task(void)
{
    xPrintTaskHandle = xTaskCreateStatic(
        print_task,
        "print_task",
        PRINT_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 2),
        xPrintTaskStack,
        &xPrintTaskBuffer
    );
}