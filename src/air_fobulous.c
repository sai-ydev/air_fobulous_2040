#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "timers.h"
#include "common.h"
#include "bsec_integration.h"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c1
#define I2C_SDA 18
#define I2C_SCL 19

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
#define WS2812_PIN  16
#endif

/* neopixel defines */
#define IS_RGBW false
#define NUM_PIXELS 1

#define BLUE_LED 25

#define LED_TASK_STACK_SIZE         500
#define NEOPIXEL_TASK_STACK_SIZE    500

#define PRINT_TASK_STACK_SIZE       500
#define SD_CARD_TASK_STACK_SIZE     500

#define BME68X_TASK_STACK_SIZE      500
#define ZMOD4x10_TASK_STACK_SIZE    500

return_values_init ret_bme68x;
struct bme68x_dev bme_dev;

/* Task Buffer Definitions */
StaticTask_t xLEDTaskBuffer;
StaticTask_t xNeopixelTaskBuffer;

StaticTask_t xPrintTaskBuffer;
StaticTask_t xSDCardTaskBuffer;

StaticTask_t xBME68xTaskBuffer;
StaticTask_t xZMOD4x10TaskBuffer;

/* Stack Definitions*/
StackType_t xLEDTaskStack[LED_TASK_STACK_SIZE];
StackType_t xNeoPixelTaskStack[NEOPIXEL_TASK_STACK_SIZE];

StackType_t xPrintTaskStack[PRINT_TASK_STACK_SIZE];
StackType_t xSDCardTaskStack[SD_CARD_TASK_STACK_SIZE];

StackType_t xBME68xTaskStack[BME68X_TASK_STACK_SIZE];
StackType_t xZMOD4x10TaskStack[ZMOD4x10_TASK_STACK_SIZE];

/* Task Handle definitions */
TaskHandle_t xLEDTaskHandle;
TaskHandle_t xNeoPixelTaskHandle;

TaskHandle_t xPrintTaskHandle;
TaskHandle_t xSDCardTaskHandle;

TaskHandle_t xBME68xTaskHandle;
TaskHandle_t xZMOD4x10TaskHandle;

SemaphoreHandle_t xI2CSemaphore;
StaticSemaphore_t xI2CSemaphore_Buffer;

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t) r << 8) | ((uint32_t) g << 16) | ((uint32_t) b);
}
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

/**
 * @brief Neopixel Task
 * @param Function
 * 
 */
 void neopixel_task(void *pvParams)
 {
    uint offset = pio_add_program(pio0, &ws2812_program);

    ws2812_program_init(pio0, 0, offset, WS2812_PIN, 800000, IS_RGBW);

    while(1)
    {
        put_pixel(urgb_u32(255, 0, 0));
        /*TODO: Neopixel drivers*/
        vTaskDelay(100);

        put_pixel(urgb_u32(0, 0, 0));
        /*TODO: Neopixel drivers*/
        vTaskDelay(100);

        put_pixel(urgb_u32(0, 255, 0));
        /*TODO: Neopixel drivers*/
        vTaskDelay(100);
        
        put_pixel(urgb_u32(0, 0, 0));
        /*TODO: Neopixel drivers*/
        vTaskDelay(100);

        put_pixel(urgb_u32(0, 0, 255));
        /*TODO: Neopixel drivers*/
        vTaskDelay(100);
        
        put_pixel(urgb_u32(0, 0, 0));
        /*TODO: Neopixel drivers*/
        vTaskDelay(100);
        
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
        printf("Print Task Current Core: %u\n", portGET_CORE_ID());
        vTaskDelay(250);
    }
}

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
    if(xI2CSemaphore != NULL)
    {
        printf("Failed to create semaphore handle\n");
        while(1);
    }


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

    xNeoPixelTaskHandle = xTaskCreateStatic(
        neopixel_task,
        "NeoPixel Task",
        NEOPIXEL_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 4),
        xNeoPixelTaskStack,
        &xNeopixelTaskBuffer
    );

    xPrintTaskHandle = xTaskCreateStatic(
        print_task,
        "print_task",
        PRINT_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 2),
        xPrintTaskStack,
        &xPrintTaskBuffer
    );

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
