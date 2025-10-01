// hal.h - Hardware Abstraction Layer
#ifndef HAL_H
#define HAL_H

#include <Arduino.h>

// RS485 Transceiver Control (if using DE/RE pins)
#define RS485_DE_PIN PB6    // Driver Enable
#define RS485_RE_PIN PB7    // Receiver Enable

// Hardware initialization
void hal_init();
void rs485_tx_mode();
void rs485_rx_mode();

#endif

// hal.cpp
#include "hal.h"

void hal_init() {
  // Initialize RS485 control pins
  pinMode(RS485_DE_PIN, OUTPUT);
  pinMode(RS485_RE_PIN, OUTPUT);
  
  // Set to receive mode by default
  rs485_rx_mode();
}

void rs485_tx_mode() {
  digitalWrite(RS485_DE_PIN, HIGH);
  digitalWrite(RS485_RE_PIN, HIGH);
}

void rs485_rx_mode() {
  digitalWrite(RS485_DE_PIN, LOW);
  digitalWrite(RS485_RE_PIN, LOW);
}