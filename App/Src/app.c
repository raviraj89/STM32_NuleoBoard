#include "app.h"
#include "blinky.h"
#include "bsp_led.h"
#include "bsp_time.h"

static blinky_t s_blinky;

void app_init(void)
{
    blinky_init(&s_blinky, APP_BLINK_INTERVAL_MS, bsp_time_now_ms());
    bsp_led_set(BSP_LED_GREEN, false);
}

void app_step(void)
{
    if (blinky_update(&s_blinky, bsp_time_now_ms())) {
        bsp_led_set(BSP_LED_GREEN, blinky_is_on(&s_blinky));
    }
}