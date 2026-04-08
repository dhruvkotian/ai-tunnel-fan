# Setup Guide - AI Tunnel Fan Monitoring (ESP32)

## Hardware Requirements

* ESP32 Development Board
* DHT22 Temperature Sensor
* Jumper Wires

---

## 🔌 Circuit Connections

| DHT22 Pin | ESP32 Pin |
| --------- | --------- |
| VCC       | 3.3V      |
| GND       | GND       |
| DATA      | GPIO 2    |

---

## Software Requirements

* Arduino IDE
* ESP32 Board Package
* TensorFlow Lite Micro Library
* DHT Sensor Library

---

## Installation Steps

### 1. Install ESP32 Board in Arduino IDE

* Go to **File → Preferences**
* Add:

  ```
  https://dl.espressif.com/dl/package_esp32_index.json
  ```
* Go to **Boards Manager → Install ESP32**

---

### 2. Install Required Libraries

Install via Library Manager:

* DHT sensor library
* Adafruit Unified Sensor
* TensorFlow Lite for Microcontrollers

---

### 3. Upload the Code

* Open `ai_tunnel_fan.ino`
* Select ESP32 board
* Select correct COM port
* Click Upload

---

### 4. Open Serial Monitor

* Baud Rate: **115200**
* Observe temperature and predictions

---

## Troubleshooting

### Issue: "Failed to read temperature"

* Check wiring
* Ensure correct GPIO pin

### Issue: "Tensor allocation failed"

* Increase tensor arena size

### Issue: Model not working

* Verify `model_data.h` and `.cc` files

---

## Expected Output

```
Measured Temperature: 30.25
Prediction: Normal Temperature.
```

---
