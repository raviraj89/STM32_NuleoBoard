#ifndef BLINKY_H
#define BLINKY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t interval_ms;
    uint32_t last_toggle_ms;
    bool     led_on;
} blinky_t;

void blinky_init(blinky_t *b, uint32_t interval_ms, uint32_t now_ms);
bool blinky_update(blinky_t *b, uint32_t now_ms);   /* true when the LED changed */
bool blinky_is_on(const blinky_t *b);

#endif /* BLINKY_H */