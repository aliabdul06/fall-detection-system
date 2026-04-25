# Fall Detection System — Health IoT Project

Group Members: Abdul Ali, Zerdani Ines Lylia, Tzini Samoili, Nishi Macwan

An autonomous IoT system that detects falls in real time using an ESP32 microcontroller and an MPU6050 accelerometer, displays alerts on a local OLED screen, and streams data to a Python dashboard over MQTT.

---

## Table of Contents

- [Overview](#overview)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Setup & Installation](#setup--installation)
- [Running the Project](#running-the-project)
- [How It Works](#how-it-works)

---

## Overview

This project targets fall detection for elderly individuals in domestic environments. When a fall is detected (acceleration exceeding 2.5g), the system:

- Displays a **FALL alert** on a local SSD1306 OLED screen
- Publishes an **MQTT message** to a broker running on a second ESP32
- Updates a **Python dashboard** with real time status
- Logs all events to a **CSV file** for later review

---

## Hardware Requirements

| Component | Quantity | Role |
|---|---|---|
| ESP32 (e.g. WROOM-32) | 2 | One client, one MQTT broker |
| MPU6050 (accelerometer/gyro) | 1 | Fall detection sensor |
| SSD1306 OLED (128x64, I2C) | 1 | Local status display |
| Jumper wires | — | Connections |
| USB cables | 2 | Power + flashing |

---

## Software Requirements

### Arduino (ESP32 boards)

Install these libraries via the Arduino Library Manager:

- `Adafruit MPU6050`
- `Adafruit SSD1306`
- `Adafruit GFX Library`
- `PubSubClient` (for the client ESP32)
- `uMQTTBroker` (for the broker ESP32)
- `Wire` (built-in)

Also install the **ESP32 board package** in Arduino IDE:  
`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

### Python (Dashboard)

Python 3.8+ required. Install dependencies:

```bash
pip install paho-mqtt
```

## Setup & Installation

### 1. Clone the repository

```bash
git clone https://github.com/aliabdul06/fall-detection-system.git
cd fall-detection-system
```

### 2. Flash the Broker ESP32

Open `esp32_broker/main.cpp` in your editor.

Update the WiFi credentials:

```cpp
const char* SSID     = "YOUR_WIFI_SSID";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";
```

Flash to the **first ESP32**. Open Serial Monitor (115200 baud) and note the IP address printed, e.g.:

```
Broker IP: 192.xxx.xx.xx
```

### 3. Flash the Client ESP32

Open `esp32_client/main.cpp` in your editor.

Update the WiFi credentials and broker IP:

```cpp
const char* SSID      = "YOUR_WIFI_SSID";
const char* PASSWORD  = "YOUR_WIFI_PASSWORD";
const char* BROKER_IP = "YOUR_BROKER_IP";  // ← IP from step 2
```

Flash to the **second ESP32** (the one with MPU6050 and OLED attached).

### 4. Configure the Python Dashboard

Open `user_dashboard/dashboard.py` and update the broker IP to match step 2:

```python
BROKER_IP = "YOUR_BROKER_IP"
```

---

## Running the Project

1. **Power on the broker ESP32** — confirm it prints its IP in Serial Monitor
2. **Power on the client ESP32** — confirm it prints `Connected to broker!` in Serial Monitor
3. **Run the dashboard** on your PC (must be on the same WiFi network):

```bash
python user_dashboard/dashboard.py
```

4. The dashboard window opens. **Tilt or shake the MPU6050** sharply to simulate a fall — the acceleration must exceed 2.5g to trigger an alert.

The dashboard will show:
- `OK` in green during normal operation
- `FALL` in red when a fall is detected
- All events saved to `fall_log.csv`

---

## How It Works

The client ESP32 continuously reads the MPU6050 accelerometer and computes the total acceleration magnitude:

$$A = \sqrt{a_x^2 + a_y^2 + a_z^2}$$

If `A > 2.5g`, a fall is detected. The ESP32 then:
- Displays `FALL !` in large text on the OLED
- Publishes `ALERT: FALL` to the MQTT topic `fall/detection`

The Python dashboard subscribes to this topic via the broker and updates the UI in real time.

---
