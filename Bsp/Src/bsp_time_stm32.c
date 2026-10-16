#include "bsp_time.h"
#include "stm32h7xx_hal.h"

uint32_t bsp_time_now_ms(void)
{
    return HAL_GetTick();
}