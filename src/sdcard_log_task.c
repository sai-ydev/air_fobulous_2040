#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define SD_CARD_TASK_STACK_SIZE     500

StaticTask_t xSDCardTaskBuffer;
StackType_t xSDCardTaskStack[SD_CARD_TASK_STACK_SIZE];
TaskHandle_t xSDCardTaskHandle;

void sdcard_log_task(void *pvParams)
{
    while(1)
    {
        //printf("SD Card Logging Task Core: %u\n", portGET_CORE_ID());
        vTaskDelay(250);
    }
}

void init_sdcard_log_task(void)
{
    xSDCardTaskHandle = xTaskCreateStatic(
        sdcard_log_task,
        "SDCard_Log_Task",
        SD_CARD_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 6),
        xSDCardTaskStack,
        &xSDCardTaskBuffer
    );
}