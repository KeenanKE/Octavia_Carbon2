// diagnostics.cpp
#include "diagnostics.h"

uint32_t modbus_last_rx = 0;
uint32_t ethernet_last_rx = 0;
bool system_fault = false;

void diag_init() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED off (active low)
  
  modbus_last_rx = millis();
  ethernet_last_rx = millis();
}

void update_modbus_timestamp() {
  modbus_last_rx = millis();
}

void update_ethernet_timestamp() {
  ethernet_last_rx = millis();
}

void watchdog_task(void *pvParameters) {
  const uint32_t TIMEOUT_MS = 10000;  // 10 second timeout
  
  while (1) {
    uint32_t now = millis();
    system_fault = false;
    
    // Check for Modbus timeout
    if ((now - modbus_last_rx) > TIMEOUT_MS) {
      system_fault = true;
    }
    
    // Check for Ethernet timeout
    if ((now - ethernet_last_rx) > TIMEOUT_MS) {
      system_fault = true;
    }
    
    // Blink LED pattern based on system status
    if (system_fault) {
      // Fast blink on fault
      digitalWrite(LED_PIN, LOW);   // LED on
      vTaskDelay(pdMS_TO_TICKS(200));
      digitalWrite(LED_PIN, HIGH);  // LED off
      vTaskDelay(pdMS_TO_TICKS(200));
    } else {
      // Slow heartbeat when healthy
      digitalWrite(LED_PIN, LOW);   // LED on
      vTaskDelay(pdMS_TO_TICKS(100));
      digitalWrite(LED_PIN, HIGH);  // LED off
      vTaskDelay(pdMS_TO_TICKS(1900));
    }
  }
}