#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "neopixel_task.h"

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

/* neopixel used in this board is GRB*/
const neopixel_color_t neopixel_color[NEOPIXEL_COLOR_MAX] ={
    {0, 0, 0, 0}, /* Black */
    {0, 0, 255, 0}, /* RED */
    {0, 0, 0, 255}, /* GREEN */
    {0, 255, 0, 0}, /* BLUE */
    {0, 0, 220, 50}, /* ORANGE */
    {0, 255, 0, 255}, /* CYAN */
    {0, 0, 255, 255}, /* YELLOW */
    {0, 180, 30, 0}, /* INDIGO */
    {0, 255, 169, 0}, /* VIOLET */
    {0, 255, 255, 0}, /* PURPLE */
    {0, 255, 255, 255} /* WHITE */
};

const char* neopixel_color_names[NEOPIXEL_COLOR_MAX] = {
    "Black",
    "Red",
    "Green",
    "Blue",
    "Orange",
    "Cyan",
    "Yellow",
    "Indigo",
    "Violet",
    "Purple",
    "White"
};

/* play colors of user's choice */
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

/* play colors defined in neopixel_task.h */
static void put_color(neopixel_color_t color)
{
    uint32_t c = *((uint32_t*) &color);
    pio_sm_put_blocking(pio0, 0, c);
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
        for(e_neopixel_color_names color_name = BLACK; color_name < NEOPIXEL_COLOR_MAX; color_name++)
        {
            printf("Playing color %s\n", neopixel_color_names[color_name]);
            put_color(neopixel_color[color_name]);
            vTaskDelay(2500);

            put_color(neopixel_color[BLACK]);
            vTaskDelay(2500); 

        }
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

