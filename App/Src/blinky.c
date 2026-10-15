#include "blinky.h"

void blinky_init(blinky_t *b, uint32_t interval_ms, uint32_t now_ms)
{
    b->interval_ms    = (interval_ms == 0u) ? 1u : interval_ms;
    b->last_toggle_ms = now_ms;
    b->led_on         = false;
}

bool blinky_update(blinky_t *b, uint32_t now_ms)
{
    /* Unsigned subtraction is correct even when the tick counter wraps. */
    if ((uint32_t)(now_ms - b->last_toggle_ms) < b->interval_ms) {
        return false;
    }
    b->last_toggle_ms += b->interval_ms;   /* stay on the grid: no drift */
    b->led_on = !b->led_on;
    return true;
}

bool blinky_is_on(const blinky_t *b)
{
    return b->led_on;
}