// diagnostics.h
#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include "config.h"

#define LED_PIN PC13  // Onboard LED

void diag_init();
void watchdog_task(void *pvParameters);

#endif

// diagnostics.cpp
#include "diagnostics.h"

uint32_t modbus_timeout = 0;
uint32_t ethernet_timeout = 0;
bool system_fault = false;

void diag_init() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED off initially
}

void watchdog_task(void *pvParameters) {
  while (1) {
    // Check for communication timeouts
    if ((millis() - modbus_timeout) > 5000) {
      system_fault = true;
    }
    
    if ((millis() - ethernet_timeout) > 5000) {
      system_fault = true;
    }
    
    // Blink LED if fault detected
    if (system_fault) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      vTaskDelay(pdMS_TO_TICKS(200));
    } else {
      digitalWrite(LED_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}