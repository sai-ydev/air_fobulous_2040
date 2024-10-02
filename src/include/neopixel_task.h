
#ifndef NEOPIXEL_TASK_H
#define NEOPIXEL_TASK_H

#include <stdlib.h> 

#define PACKED_STRUCT struct __packed 

/* bitfield struct to store color information*/
typedef PACKED_STRUCT
{
    /* data */
    uint32_t white:8; /* current neopixel is rgb*/
    uint32_t blue:8;
    uint32_t red:8;
    uint32_t green:8;
} neopixel_color_t;

typedef enum
{
    BLACK = 0,
    RED,
    GREEN,
    BLUE,
    ORANGE,
    CYAN,
    YELLOW,
    INDIGO,
    VIOLET,
    PURPLE,
    WHITE,


    
    NEOPIXEL_COLOR_MAX,

} e_neopixel_color_names;

typedef enum
{
    NEOPIXEL_AQ_IDLE = 0, /* neopixel turned off */
    NEOPIXEL_AQ_BOOTUP,
    NEOPIXEL_AQ_GOOD, /* Green */
    NEOPIXEL_AQ_MODERATE,
    NEOPIXEL_AQ_UNHEALTHY,
    NEOPIXEL_AQ_VERY_UNHEALTHY,

    NEOPIXEL_AQ_MAX
} e_neopixel_air_quality;


/* enum used to change LED behavior 
  based on the device status */
typedef enum
{
    NEOPIXEL_STATE_IDLE = 0,
    NEOPIXEL_STATE_BOOTUP,
    NEOPIXEL_STATE_MEASURE_ONE, /* one of the sensors is measuring data */
    NEOPIXEL_STATE_MEASURE_BOTH,
    NEOPIXEL_STATE_DISPLAY_AQ,

    NEOPIXEL_STATE_MAX

} e_neopixel_state;

void init_neopixel_task(void);

#endif