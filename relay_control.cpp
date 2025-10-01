// relay_control.cpp
#include "relay_control.h"

SemaphoreHandle_t relay_mutex;
QueueHandle_t cmd_queue;
uint32_t relay_state = 0;

void relay_init() {
  relay_mutex = xSemaphoreCreateMutex();
  cmd_queue = xQueueCreate(16, sizeof(RelayCmd));
  
  for (uint8_t i = 0; i < 32; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], LOW);
  }
}

void relay_set(uint8_t num, bool state) {
  if (num >= 32) return;
  
  if (xSemaphoreTake(relay_mutex, pdMS_TO_TICKS(100))) {
    digitalWrite(RELAY_PINS[num], state ? HIGH : LOW);
    if (state) relay_state |= (1UL << num);
    else relay_state &= ~(1UL << num);
    xSemaphoreGive(relay_mutex);
  }
}

void relay_set_bank(uint8_t bank, uint8_t mask) {
  if (bank >= 4) return;
  uint8_t base = bank * 8;
  
  for (uint8_t i = 0; i < 8; i++) {
    relay_set(base + i, mask & (1 << i));
  }
}

bool relay_get(uint8_t num) {
  return (num < 32) ? (relay_state & (1UL << num)) != 0 : false;
}

uint32_t relay_get_all() {
  return relay_state;
}

void relay_task(void *pvParameters) {
  RelayCmd cmd;
  
  while (1) {
    if (xQueueReceive(cmd_queue, &cmd, pdMS_TO_TICKS(10))) {
      relay_set(cmd.relay_num, cmd.state);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}