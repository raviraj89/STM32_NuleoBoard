#ifndef BSP_LED_H
#define BSP_LED_H

#include <stdbool.h>

typedef enum {
    BSP_LED_GREEN = 0,   /* LD1, PB0 */
    BSP_LED_YELLOW,      /* LD2, PB7 */
    BSP_LED_RED,         /* LD3, PB14 */
    BSP_LED_COUNT
} bsp_led_t;

void bsp_led_set(bsp_led_t led, bool on);

#endif /* BSP_LED_H */