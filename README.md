# ESP32-Based Smart Industrial IoT Monitoring and Automatic Cooling System Using I2C,WiFi and Telegram Bot

## 📌 Project Overview

This project is an ESP32-based Industrial IoT Monitoring and Automatic Cooling System designed to monitor environmental conditions and provide automatic cooling with remote monitoring capabilities.

The system uses a DHT11 sensor to measure temperature and humidity. Based on the temperature threshold, the ESP32 automatically controls a 12V DC cooling fan through a relay module.

An I2C LCD displays real-time sensor readings, while Wi-Fi connectivity enables remote monitoring and fan control through a Telegram Bot. An IR sensor is also integrated for intrusion detection and instant Telegram alerts.

## 🚀 Features

* Real-time temperature and humidity monitoring.
* Automatic cooling fan control based on temperature.
* I2C-based 16x2 LCD display.
* Wi-Fi connectivity using ESP32.
* Telegram Bot integration for remote monitoring.
* Manual fan ON/OFF control through Telegram.
* Automatic and Manual operating modes.
* IR-based intrusion detection.
* Instant Telegram notifications.
* LED indicators for system status.

## 🛠️ Hardware Components

| Component                           |    Quantity |
| ----------------------------------- | ----------: |
| ESP32 DevKit                        |           1 |
| DHT11 Temperature & Humidity Sensor |           1 |
| 16x2 LCD with I2C Module            |           1 |
| 1-Channel Relay Module              |           1 |
| 12V DC Fan                          |           1 |
| IR Sensor Module                    |           1 |
| Green LED                           |           1 |
| Red LED                             |           1 |
| 220Ω Resistors                      |           2 |
| 12V DC Adapter                      |           1 |
| Breadboard and Jumper Wires         | As required |

## 💻 Software & Technologies

* Embedded C / Arduino Framework
* ESP32 Microcontroller
* Visual Studio Code
* PlatformIO IDE
* Arduino Libraries
* I2C Communication
* Wi-Fi Communication
* Telegram Bot API

## 🔌 GPIO Pin Configuration

| ESP32 GPIO | Connected Component |
| ---------- | ------------------- |
| GPIO 4     | DHT11 Data          |
| GPIO 18    | Green LED           |
| GPIO 19    | Red LED             |
| GPIO 21    | LCD SDA             |
| GPIO 22    | LCD SCL             |
| GPIO 26    | Relay IN            |
| GPIO 27    | IR Sensor OUT       |

## ⚙️ Working Principle

1. The DHT11 sensor continuously measures temperature and humidity.
2. The ESP32 reads the sensor data and displays it on the I2C LCD.
3. When the temperature reaches 30°C, the cooling fan automatically turns ON.
4. When the temperature drops to 28°C, the fan automatically turns OFF.
5. The ESP32 connects to Wi-Fi and communicates with the Telegram Bot.
6. Users can monitor system status and control the fan remotely.
7. The IR sensor detects intrusion and sends an alert through Telegram.
8. LED indicators show the fan operating status.

## 📱 Telegram Bot Commands

| Command    | Function                                 |
| ---------- | ---------------------------------------- |
| `/start`   | Start the Telegram Bot                   |
| `/status`  | Get temperature, humidity and fan status |
| `/fan_on`  | Turn ON the fan manually                 |
| `/fan_off` | Turn OFF the fan manually                |
| `/auto`    | Enable automatic cooling mode            |

## 🌡️ Automatic Temperature Control

| Temperature Condition | Fan Operation                |
| --------------------- | ---------------------------- |
| 30°C and above        | Fan ON                       |
| 28°C and below        | Fan OFF                      |
| Between 28°C and 30°C | Maintains previous fan state |

## 📺 Project Demonstration

**YouTube Demo Video:**

[Watch Project Demonstration] https://youtu.be/_iU4UhEMqEY?si=o2EC78AHHEPztMBa

## 📂 Project Structure

```text
ESP32-Industrial-IoT-Monitoring/
│
├── src/
│   └── main.cpp
│
├── platformio.ini
│
├── README.md
│
└── images/
    ├── project_setup.jpg
    └── circuit_diagram.png
```

## 🔮 Future Improvements

* Cloud-based data logging.
* Web dashboard for real-time monitoring.
* Multiple sensor integration.
* Industrial-grade temperature sensors.
* Data visualization and historical monitoring.

## 👨‍💻 Author

**Bala Krishnan M.**

Embedded Systems | ESP32 | Embedded C | IoT

## 📜 License

This project is intended for educational and learning purposes.
