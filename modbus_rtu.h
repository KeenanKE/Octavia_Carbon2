// modbus_rtu.h
#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "config.h"

void modbus_init();
void modbus_task(void *pvParameters);
void modbus_process();

#endif

// modbus_rtu.cpp
#include "modbus_rtu.h"
#include "relay_control.h"

uint8_t rx_buffer[256];
uint8_t rx_index = 0;
uint32_t last_rx_time = 0;

uint16_t calc_crc(uint8_t *buf, uint8_t len) {
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= buf[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x0001) crc = (crc >> 1) ^ 0xA001;
      else crc >>= 1;
    }
  }
  return crc;
}

void modbus_init() {
  MODBUS_SERIAL.begin(MODBUS_BAUDRATE);
}

void modbus_send_response(uint8_t *data, uint8_t len) {
  uint16_t crc = calc_crc(data, len);
  data[len++] = crc & 0xFF;
  data[len++] = crc >> 8;
  MODBUS_SERIAL.write(data, len);
}

void modbus_process() {
  if (rx_index < 8) return;
  
  uint8_t slave_id = rx_buffer[0];
  uint8_t func = rx_buffer[1];
  
  if (slave_id != MODBUS_SLAVE_ID) {
    rx_index = 0;
    return;
  }
  
  uint16_t crc_rx = rx_buffer[rx_index-2] | (rx_buffer[rx_index-1] << 8);
  uint16_t crc_calc = calc_crc(rx_buffer, rx_index - 2);
  
  if (crc_rx != crc_calc) {
    rx_index = 0;
    return;
  }
  
  uint8_t response[256];
  uint8_t resp_len = 0;
  
  switch (func) {
    case 0x01: { // Read Coils
      uint16_t addr = (rx_buffer[2] << 8) | rx_buffer[3];
      uint16_t qty = (rx_buffer[4] << 8) | rx_buffer[5];
      
      response[0] = slave_id;
      response[1] = func;
      response[2] = (qty + 7) / 8;
      
      uint32_t state = relay_get_all();
      for (uint8_t i = 0; i < response[2]; i++) {
        response[3 + i] = (state >> (addr + i*8)) & 0xFF;
      }
      resp_len = 3 + response[2];
      modbus_send_response(response, resp_len);
      break;
    }
    
    case 0x05: { // Write Single Coil
      uint16_t addr = (rx_buffer[2] << 8) | rx_buffer[3];
      bool state = rx_buffer[4] == 0xFF;
      
      relay_set(addr, state);
      
      for (uint8_t i = 0; i < 6; i++) response[i] = rx_buffer[i];
      resp_len = 6;
      modbus_send_response(response, resp_len);
      break;
    }
    
    case 0x0F: { // Write Multiple Coils
      uint16_t addr = (rx_buffer[2] << 8) | rx_buffer[3];
      uint16_t qty = (rx_buffer[4] << 8) | rx_buffer[5];
      
      for (uint16_t i = 0; i < qty; i++) {
        uint8_t byte_idx = i / 8;
        uint8_t bit_idx = i % 8;
        bool state = (rx_buffer[7 + byte_idx] >> bit_idx) & 0x01;
        relay_set(addr + i, state);
      }
      
      for (uint8_t i = 0; i < 6; i++) response[i] = rx_buffer[i];
      resp_len = 6;
      modbus_send_response(response, resp_len);
      break;
    }
  }
  
  rx_index = 0;
}

void modbus_task(void *pvParameters) {
  while (1) {
    while (MODBUS_SERIAL.available()) {
      rx_buffer[rx_index++] = MODBUS_SERIAL.read();
      last_rx_time = millis();
      if (rx_index >= 256) rx_index = 0;
    }
    
    if (rx_index > 0 && (millis() - last_rx_time) > 10) {
      modbus_process();
    }
    
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}