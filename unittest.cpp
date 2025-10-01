#include <unity.h>
#include "relay_control.h"

void test_relay_init() {
    relay_init();
    TEST_ASSERT_EQUAL(0, relay_get_all());
}

void test_relay_set() {
    relay_set(0, true);
    TEST_ASSERT_TRUE(relay_get(0));
    relay_set(0, false);
    TEST_ASSERT_FALSE(relay_get(0));
}

void test_relay_set_bank() {
    relay_set_bank(0, 0xFF);
    TEST_ASSERT_EQUAL(0xFF, relay_get_all() & 0xFF);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_relay_init);
    RUN_TEST(test_relay_set);
    RUN_TEST(test_relay_set_bank);
    UNITY_END();
}

void loop() {}