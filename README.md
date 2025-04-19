# 🌿 Smart AgroLoRa: LoRa-Based Environmental Monitoring System

A real-time wireless environmental monitoring system for smart agriculture using **LoRa**, **DHT11**, **Soil Moisture Sensor**, and **ThingSpeak**. This project demonstrates a complete **IoT-based** solution that collects and transmits environmental data (temperature, humidity, and soil moisture) over long distances using **LoRa communication**, with cloud integration and alert mechanisms.

## 🚀 Features

- 📡 **LoRa Communication** between transmitter and receiver modules (433 MHz).
- 🌡️ **Environmental Sensing** using DHT11 (Temperature and Humidity) & Soil Moisture sensor.
- 💡 **Visual Alerts** using LEDs for unhealthy environmental conditions.
- 📺 **Real-time LCD Display** (I2C 16x2) on receiver end.
- ☁️ **Cloud Integration** with [ThingSpeak](https://thingspeak.com/) for data visualization.
- 📶 **WiFi-Enabled Receiver** using ESP32 with automatic reconnect.
- 🔄 **Robust Data Parsing** and error handling.
- 🛠️ Modular, extendable, and open for improvements.

---

## 🧠 System Architecture

```
[Sensors (DHT11, Soil)] 
       ↓
[LoRa Transmitter - Arduino UNO]
       ↓ 433 MHz
[LoRa Receiver - ESP32]
       ↓
[LCD Display] + [ThingSpeak Upload] + [LED Alerts]
```

---

## 🔌 Hardware Components

| Component | Quantity |
|----------|----------|
| Arduino UNO | 1 |
| ESP32 | 1 |
| LoRa SX1278 Module | 2 |
| DHT11 Temperature & Humidity Sensor | 1 |
| Soil Moisture Sensor | 1 |
| I2C LCD Display (16x2) | 1 |
| LEDs (for alert/indicator) | 2 |
| Resistors, Breadboard, Jumper Wires | As required |

---

## 📦 Repository Structure

```
📁 Smart-AgroLoRa/
├── transmitter_code.ino        # Arduino UNO (Transmitter)
├── receiver_code.ino           # ESP32 (Receiver + LCD + WiFi)
├── README.md                   # Project Documentation
```

---

## 📡 Setup & Deployment

### 🛠 Transmitter (Arduino UNO)
1. Connect DHT11 and soil sensor to Arduino.
2. Connect LoRa module (ensure correct SPI pins).
3. Upload `transmitter_code.ino`.

### 📶 Receiver (ESP32)
1. Connect LoRa and I2C LCD to ESP32.
2. Set WiFi credentials and your ThingSpeak API key in `receiver_code.ino`.
3. Upload the code and power it up.

### 🌐 ThingSpeak
- Create a channel with **three fields** (Temperature, Humidity, Moisture).
- Replace the API key in the code with your channel's write API key.

---

## 📊 Sample Output (Serial Monitor)

**Transmitter:**
```
Sent: T:26.4,H:56.1,M:845
```

**Receiver:**
```
Received: T:26.4,H:56.1,M:845
ThingSpeak Response: 200
```

LCD:
```
T: 26.4C
H: 56.1% M:845
```

---

## 🧪 Threshold Configuration

```cpp
// Safe Ranges
float tempLow = 15, tempHigh = 35;
float humLow = 30, humHigh = 80;
float moistLow = 300, moistHigh = 1200;
```

LED Alert turns ON if any sensor data goes beyond the configured limits.

---

## 🧠 Future Enhancements

- Integrate GPS for geo-tagging sensor locations.
- Add relay control for irrigation systems.
- Implement LoRa mesh for larger coverage.
- Add OTA updates for ESP32.
- Create a dashboard with historical data visualization.

---

## 📸 Preview

> *(Add images/gifs of your setup, serial monitor, and LCD screen here for better appeal)*

---

## 🤝 Contribution

Pull requests are welcome! Feel free to open issues for suggestions or bugs.

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙌 Acknowledgements

- [ThingSpeak IoT Platform](https://thingspeak.com/)
- [LoRa by Semtech](https://www.semtech.com/lora)
- [DHT Sensor Library by Adafruit](https://github.com/adafruit/DHT-sensor-library)

---

> Designed & Developed by **Sayantan** 👨‍💻 | Made with ❤️ for Smart Agriculture 🌱