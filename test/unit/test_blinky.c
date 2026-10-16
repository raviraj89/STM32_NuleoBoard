#include <stdint.h>
#include "unity.h"
#include "blinky.h"

static blinky_t b;

void setUp(void)    { blinky_init(&b, 500u, 0u); }
void tearDown(void) {}

void test_led_starts_off(void)
{
    TEST_ASSERT_FALSE(blinky_is_on(&b));
}

void test_no_toggle_before_interval(void)
{
    TEST_ASSERT_FALSE(blinky_update(&b, 499u));
    TEST_ASSERT_FALSE(blinky_is_on(&b));
}

void test_toggles_on_then_off(void)
{
    TEST_ASSERT_TRUE(blinky_update(&b, 500u));
    TEST_ASSERT_TRUE(blinky_is_on(&b));
    TEST_ASSERT_TRUE(blinky_update(&b, 1000u));
    TEST_ASSERT_FALSE(blinky_is_on(&b));
}

void test_late_update_does_not_drift(void)
{
    blinky_update(&b, 520u);                      /* main loop ran 20 ms late */
    TEST_ASSERT_FALSE(blinky_update(&b, 999u));
    TEST_ASSERT_TRUE(blinky_update(&b, 1000u));   /* still on the 500 ms grid */
}

void test_survives_tick_wraparound(void)
{
    blinky_init(&b, 500u, UINT32_MAX - 100u);
    TEST_ASSERT_FALSE(blinky_update(&b, 300u));   /* 401 ms elapsed across the wrap */
    TEST_ASSERT_TRUE(blinky_update(&b, 399u));    /* exactly 500 ms */
}

void test_zero_interval_is_clamped(void)
{
    blinky_init(&b, 0u, 0u);
    TEST_ASSERT_FALSE(blinky_update(&b, 0u));
    TEST_ASSERT_TRUE(blinky_update(&b, 1u));
}