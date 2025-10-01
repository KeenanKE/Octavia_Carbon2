// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>

// Relay Banks Pin Mapping (STM32F407)
#define BANK0_BASE PB10  // QX0.0-QX0.7
#define BANK1_BASE PC13  // QX1.0-QX1.7
#define BANK2_BASE PA0   // QX2.0-QX2.7
#define BANK3_BASE PA8   // QX3.0-QX3.7

const uint8_t RELAY_PINS[32] = {
  PB10, PB12, PB13, PB14, PB15, PC13, PC14, PC15,  // Bank 0
  PA0, PA1, PA4, PA5, PA6, PA7, PB0, PB1,          // Bank 1
  PA8, PA11, PA12, PB3, PB4, PB5, PB8, PB9,        // Bank 2
  PA15, PC10, PC11, PC12, PD2, PA9, PA10, PA13     // Bank 3
};

// Communication
#define MODBUS_SERIAL Serial1
#define MODBUS_BAUDRATE 115200
#define MODBUS_SLAVE_ID 1

// Synchronization
extern SemaphoreHandle_t relay_mutex;
extern QueueHandle_t cmd_queue;

// Relay state buffer
extern uint32_t relay_state;

#endif