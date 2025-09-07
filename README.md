# IoT ESP32 Project

This project demonstrates controlling an **ESP32** over MQTT using a **Python GUI** built with Tkinter.  
It uses **Wokwi** as the ESP32 simulator and **PlatformIO** in VS Code for development.  <br><br>

**HiveMQ** : Broker<br>
**MQTTX** : MQTT Client tool (see incoming payload)

---

## Features
- ESP32 connects to **HiveMQ public MQTT broker** (`broker.hivemq.com`)
- Subscribes to topic `IoT_Undip_ESP32`
- Turns onboard LEDs / LED Strips **ON** or **OFF** or **Different Pattern** depending on MQTT message
- Python Tkinter GUI with buttons to:
  - Turn LEDs **ON** : 1
  - Turn LEDs **OFF** : 0
  - Show current LED status
  - Set Patterns to **Rainbow** : 2
  - Set Patterns to **Snake** : 3
  - Set Patterns to **Fast** : 4
  - Set Patterns to **Arrow** : 5
  - Publish to `IoT_Undip_ESP32` with 0 - 5 payload 
- Simulated in **Wokwi** for quick testing

## Default Template
You can find the default template ie. simple turn on turn off LED at : ESP32_Simple<br>

### Default ESP32 - GUI
![ESP32 & GUI Screenshot](Assets/ESP32_GUI.png)

### MQTTX
![MQTTX Screenshot](Assets/MQTTX.png)

---
