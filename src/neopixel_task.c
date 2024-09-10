#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
#define WS2812_PIN  16
#endif

/* neopixel defines */
#define IS_RGBW false
#define NUM_PIXELS 1

#define NEOPIXEL_TASK_STACK_SIZE    500

StaticTask_t xNeopixelTaskBuffer;

StackType_t xNeoPixelTaskStack[NEOPIXEL_TASK_STACK_SIZE];

TaskHandle_t xNeoPixelTaskHandle;

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t) r << 8) | ((uint32_t) g << 16) | ((uint32_t) b);
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

void init_neopixel_task(void)
{
    xNeoPixelTaskHandle = xTaskCreateStatic(
        neopixel_task,
        "NeoPixel Task",
        NEOPIXEL_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 4),
        xNeoPixelTaskStack,
        &xNeopixelTaskBuffer
    );
}

