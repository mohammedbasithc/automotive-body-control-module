# 🚗 Automotive Body Control Module (BCM)
### Design and Implementation using Arduino Uno & TFT Display

![Platform](https://img.shields.io/badge/Platform-Arduino%20Uno-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-Embedded%20C%2FC%2B%2B-blue?style=for-the-badge)
![Display](https://img.shields.io/badge/Display-ST7735%20TFT-orange?style=for-the-badge)
![Domain](https://img.shields.io/badge/Domain-Automotive%20Electronics-red?style=for-the-badge)

---

## 📌 Overview

This project presents the **design and implementation of an Automotive Body Control Module (BCM)** — a simplified prototype that replicates the core functionalities of BCMs found in modern vehicles. Built using an **Arduino Uno** and an **ST7735 TFT Display**, the system manages essential vehicle body functions including lighting control, temperature monitoring, fault detection, and real-time dashboard display.

> Developed as a Mini Project for **Embedded System and IoT Design**
> **Mohammed Basith** | B.E ECE | Reg. No: 311823106036

---

## 🎯 Objectives

- Understand the fundamentals of **embedded systems** and **automotive electronics**
- Design and implement an Automotive BCM using **Arduino Uno**
- Control vehicle **headlights** and **turn indicators**
- Monitor **temperature** using the LM35 sensor
- Provide **fault detection** and alert mechanisms
- Display vehicle status information using a **TFT dashboard interface**

---

## ⚙️ Features

| Feature | Description |
|---|---|
| 🔆 Headlight Control | Toggle headlights via push button with relay output |
| 🔄 Turn Indicators | Left/right indicator LEDs controlled by dedicated switches |
| 🌡️ Temperature Monitoring | Continuous analog temperature reading via LM35 sensor |
| ⚠️ Fault Detection | Buzzer alert + TFT warning when temperature exceeds threshold |
| 📊 TFT Dashboard | Real-time display of all vehicle parameters |
| 💡 Low-Cost Design | Built with readily available, affordable components |

---

## 🧰 Components Required

| S.No | Component | Quantity | Purpose |
|------|-----------|----------|---------|
| 1 | Arduino Uno | 1 | Main microcontroller |
| 2 | ST7735 TFT Display | 1 | Dashboard / HMI interface |
| 3 | LM35 Temperature Sensor | 1 | Temperature monitoring |
| 4 | Buzzer | 2 | Fault warning alerts |
| 5 | Push Buttons | As needed | Headlight & indicator control |
| 6 | Breadboard | 1 | Prototyping platform |
| 7 | Jumper Wires | As required | Circuit connections |
| 8 | Power Supply | 1 | System power (5V) |

---

## 🖥️ Software & Tools

- **Arduino IDE** — Code development and upload
- **Embedded C/C++** — Programming language
- **SPI Communication Protocol** — TFT display interfacing
- Libraries: `Adafruit_ST7735`, `Adafruit_GFX`

---

## 🔌 Circuit Overview

```
Arduino Uno
├── TFT Display (ST7735) → SPI pins (CS, RST, DC, MOSI, SCK)
├── LM35 Temperature Sensor → Analog pin A0
├── Headlight Switch → Digital pin D2
├── Left Indicator Switch → Digital pin D3
├── Right Indicator Switch → Digital pin D4
├── Buzzer 1 → Digital pin D5
└── Buzzer 2 → Digital pin D6
```

> 📁 Refer to the `circuit/` folder for the full schematic diagram.

---

## 🚀 How to Run

1. **Clone this repository**
   ```bash
   git clone https://github.com/YOUR_USERNAME/automotive-body-control-module.git
   cd automotive-body-control-module
   ```

2. **Open in Arduino IDE**
   - Open `src/bcm_main.ino`

3. **Install required libraries** (via Arduino Library Manager)
   - `Adafruit ST7735 and ST7789 Library`
   - `Adafruit GFX Library`

4. **Connect hardware** as per the circuit diagram in `circuit/`

5. **Select board & port**
   - Board: `Arduino Uno`
   - Port: Your COM/USB port

6. **Upload the code** and open Serial Monitor at 9600 baud

---

## 🔁 Working Principle

1. Arduino continuously reads **switch inputs** and the **LM35 analog signal**
2. **Headlight switch** → activates relay and turns ON headlight LED
3. **Indicator switches** → control respective left/right LEDs
4. **LM35 sensor** → converts analog value to temperature (°C) and displays on TFT
5. If temperature **exceeds threshold**:
   - Buzzer activates
   - Fault warning displayed on TFT screen
   - Driver is alerted
6. All parameters are **continuously refreshed** on the TFT dashboard

---

## 📈 Outcomes

✅ Successfully implemented an Automotive BCM using Arduino Uno  
✅ Controlled headlights and turn indicators via embedded programming  
✅ Real-time vehicle status displayed on TFT dashboard  
✅ Abnormal temperature detection with buzzer warning alerts  
✅ Demonstrated practical automotive embedded system functionality  

---

## 🔭 Future Enhancements

- [ ] **CAN Bus communication** between ECUs
- [ ] **FreeRTOS task scheduling** for multitasking
- [ ] **Diagnostic Trouble Codes (DTC)** implementation
- [ ] **Graphical dashboard** with icons and charts
- [ ] **EV battery temperature monitoring**
- [ ] **Wireless OTA updates** via ESP32

---

## 📚 References

- [Arduino Uno Official Documentation](https://www.arduino.cc/en/Guide/ArduinoUno)
- ATmega328P Datasheet
- [Adafruit ST7735 TFT Display Documentation](https://learn.adafruit.com/1-8-tft-display)
- Raj Kamal — *Embedded Systems: Architecture, Programming and Design*
- Frank Vahid & Tony Givargis — *Embedded System Design*
- Automotive Electronics Handbook

---

## 👨‍💻 Author

**Mohammed Basith**  
B.E Electronics and Communication Engineering  
Cont No:9526908266


---

## 📄 License

This project is for **educational purposes**. Feel free to fork, modify, and build upon it with proper attribution.

---

⭐ *If you found this project helpful, consider giving it a star!*
