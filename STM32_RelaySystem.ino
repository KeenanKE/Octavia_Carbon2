// main.ino
#include <Arduino_FreeRTOS.h>
#include "config.h"
#include "relay_control.h"
#include "modbus_rtu.h"
#include "ethernet_comm.h"
#include "diagnostics.h"

void setup() {
  Serial.begin(115200);
  
  // Initialize subsystems
  relay_init();
  modbus_init();
  ethernet_init();
  diag_init();
  
  // Create FreeRTOS tasks
  xTaskCreate(relay_task, "Relay", 256, NULL, 3, NULL);
  xTaskCreate(modbus_task, "Modbus", 512, NULL, 2, NULL);
  xTaskCreate(ethernet_task, "Ethernet", 512, NULL, 2, NULL);
  xTaskCreate(watchdog_task, "Watchdog", 128, NULL, 1, NULL);
  
  vTaskStartScheduler();
}

void loop() {
  // Empty - FreeRTOS manages execution
}