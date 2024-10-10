#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "neopixel_task.h"
#include "timers.h"

#define RGB_CHANNEL_MAX 255
#define RGB_CHANNEL_MIN 0

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
#define WS2812_PIN  16
#endif

/* neopixel defines */
#define IS_RGBW false
#define NUM_PIXELS 1

#define NEOPIXEL_TASK_STACK_SIZE    500
#define NEOPIXEL_TIMER_INTERVAL_MS  pdMS_TO_TICKS(1) 
#define VAL_CHECK(x, max) x > max ? max : x

/* neopixel used in this board is GRB*/
const neopixel_color_t neopixel_color[NEOPIXEL_COLOR_MAX] ={
    {0, 0, 0, 0}, /* Black */
    {0, 0, 255, 0}, /* RED */
    {0, 0, 0, 255}, /* GREEN */
    {0, 255, 0, 0}, /* BLUE */
    {0, 0, 220, 50}, /* ORANGE */
    {0, 255, 0, 255}, /* CYAN */
    {0, 0, 255, 255}, /* YELLOW */
    {0, 255, 35, 0}, /* INDIGO */
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

StaticTask_t xNeopixelTaskBuffer;
StackType_t xNeopixelTaskStack[NEOPIXEL_TASK_STACK_SIZE];
TaskHandle_t xNeopixelTaskHandle;

TimerHandle_t xNeopixelTimerHandle;
StaticTimer_t xNeopixelTimerBuffer;

e_neopixel_air_quality air_quality = NEOPIXEL_AQ_IDLE;
e_neopixel_state neopixel_state = NEOPIXEL_AQ_IDLE;

/* play colors of user's choice */
void put_pixel(uint32_t pixel_grb);
/* play colors defined in neopixel_task.h */
void put_color(neopixel_color_t color);
/* Build RGB color*/
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
/* breathe mode - breathes the neopixel based on the color chosen*/
void breathe_mode(neopixel_color_t color);

void process_led_events();

void vNeopixelTimerCallback(TimerHandle_t xTimer);

/**
 * @brief Neopixel Task
 * @param Function
 * 
 */
void neopixel_task(void *pvParams)
{
    uint offset = pio_add_program(pio0, &ws2812_program);

    ws2812_program_init(pio0, 0, offset, WS2812_PIN, 800000, IS_RGBW);

    //turn LED off
    put_color(neopixel_color[BLACK]);

    while(1)
    {
        
    }
}

void init_neopixel_task(void)
{
    xNeopixelTaskHandle = xTaskCreateStatic(
        neopixel_task,
        "NeoPixel Task",
        NEOPIXEL_TASK_STACK_SIZE,
        NULL,
        (tskIDLE_PRIORITY + 8),
        xNeopixelTaskStack,
        &xNeopixelTaskBuffer
    );

    xNeopixelTimerHandle = xTimerCreateStatic(
        "NeoPixel Timer",
        NEOPIXEL_TIMER_INTERVAL_MS,
        pdTRUE,
        (void*) 0,
        vNeopixelTimerCallback,
        &xNeopixelTimerBuffer
    );
}

/* play colors of user's choice */
void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

/* play colors defined in neopixel_task.h */
void put_color(neopixel_color_t color)
{
    uint32_t c = *((uint32_t*) &color);
    pio_sm_put_blocking(pio0, 0, c);
}

/* Build RGB color*/
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t) r << 8) | ((uint32_t) g << 16) | ((uint32_t) b);
}

/* breathe mode - breathes the neopixel based on the color chosen*/
void breathe_mode(neopixel_color_t color)
{
    if(memcmp(&color, &neopixel_color[BLACK], sizeof(neopixel_color_t)) == 0) return;
    
    uint8_t red_max = color.red;
    uint8_t blue_max = color.blue;
    uint8_t green_max = color.green;

    uint8_t r_val;
    uint8_t b_val;
    uint8_t g_val;

    for(uint8_t i = RGB_CHANNEL_MIN; i < RGB_CHANNEL_MAX; i++)
    {
        r_val = VAL_CHECK(i, red_max);
        g_val = VAL_CHECK(i, green_max);
        b_val = VAL_CHECK(i, blue_max);

        put_pixel(urgb_u32(r_val, g_val, b_val));
        vTaskDelay(5);
    }
    vTaskDelay(2500);
    for(uint8_t j = RGB_CHANNEL_MAX; j > RGB_CHANNEL_MIN; j--)
    {
        r_val = VAL_CHECK(j, red_max);
        g_val = VAL_CHECK(j, green_max);
        b_val = VAL_CHECK(j, blue_max);
        
        put_pixel(urgb_u32(r_val, g_val, b_val));
        vTaskDelay(5);
    }
    vTaskDelay(1000);


}

void process_led_events()
{
    switch(neopixel_state)
    {

        default:
            break;
    }
}

/**
 * @brief timer callback function
 * 
 */
void vNeopixelTimerCallback(TimerHandle_t xTimer)
{
    
}