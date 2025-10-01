# 🛠️ Embedded Control System with Industrial Interfaces



## 📖 Overview
This project implements an **embedded control system** using an **STM32F4 microcontroller** to drive **32 solid-state relays**.  
The system supports **RS485/Modbus RTU** and **Ethernet (TCP/IP)** for integration into industrial networks.  
Instead of Siemens PLCs, this system integrates with **OpenPLC** for ladder logic and remote control.  

---

## ⚡ Hardware Design

### 🔑 Core Components
- **STM32F4** – Main controller, runs FreeRTOS tasks.  
- **74HC595** – Shift registers for relay expansion (4 × 8 relays).  
- **ULN2803A** – Darlington drivers to sink relay currents.  
- **Relays (PLC-RSC-24DC/21)** – 32 relays grouped in 4 banks of 8.  
- **RS485 (MAX485)** – Modbus RTU slave transceiver.  
- **Ethernet (LAN8720 + RJ45 w/ magnetics)** – TCP/IP comms.  
- **Opto-Isolators (PC817/TLP281)** – Galvanic isolation.  

### 🔒 Power & Protection
- **Power Chain**: 240VAC → 24VDC → 5V/3.3V (MCU + logic).  
- **Protection**: fuses, TVS diodes, snubber networks.  
- **Decoupling**: 0.1µF caps near ICs, bulk caps per rail.  

📐 Schematics provided in `/schematics` (KiCad).  

---

## 💻 Firmware Design

### 🔧 Development
- **Arduino IDE** + **STM32duino core**  
- Modular structure for hardware, comms, and logic layers  

### 🧵 FreeRTOS Tasks
- `RelayTask` – Update relay states via 74HC595 → ULN2803A  
- `ModbusTask` – Handle RS485 Modbus RTU requests  
- `EthernetTask` – TCP/IP comms (Modbus TCP/OpenPLC)  
- `SystemMonitorTask` – Watchdog, diagnostics, fault handling  

### 📜 Functions
- Relay control:  
  ```cpp
  setRelay(bank, relay);
  clearRelay(bank, relay);
  toggleRelay(bank, relay);
  readRelayState(bank, relay);
