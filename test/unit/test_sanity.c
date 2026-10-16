#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_unity_is_wired_up(void)
{
    TEST_ASSERT_EQUAL_INT(4, 2 + 2);
}