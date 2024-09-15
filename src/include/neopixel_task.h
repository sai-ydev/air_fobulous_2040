
#ifndef NEOPIXEL_TASK_H
#define NEOPIXEL_TASK_H

#include <stdlib.h> 

#define PACKED_STRUCT struct __packed 

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




void init_neopixel_task(void);

#endif