# ESP32 Voice-Controlled GPIO with Micro-ROS and PlatformIO

This project allows you to control an buggy connected to esp32 using voice commands. The program listens for specific commands like **"go"** and **"stop"**, sends HTTP requests to the ESP32 running a Micro-ROS-based firmware, and provides feedback via speech and text.

## Features

- **Voice Recognition**: Recognizes commands like "go", "stop", "quit", or "exit".
- **Signal Transmission**: Sends HTTP requests to an ESP32 to toggle GPIO pins.
- **Micro-ROS Integration**: ESP32 firmware is developed using Micro-ROS and PlatformIO.
- **Text-to-Speech Feedback**: Provides verbal feedback for actions and errors.
- **Error Handling**: Handles network and speech recognition errors gracefully.

---

## Requirements

### Hardware
- ESP32 microcontroller with Wi-Fi.
- Microphone for speech input.

### Software
- **Host PC:**
  - Python 3.x
  - Required Python libraries:
    - `requests`
    - `speech_recognition`
    - `pyttsx3`
- **ESP32:**
  - PlatformIO IDE.
  - Micro-ROS library.

---

## Setup Instructions

### ESP32 Configuration (Micro-ROS and PlatformIO)
1. **Install PlatformIO**:
   - Install PlatformIO as a VS Code extension or as a standalone IDE.
   - [PlatformIO Installation Guide](https://platformio.org/install).

2. **Create Micro-ROS Project**:
   - Start a new PlatformIO project for the ESP32.
   - Add Micro-ROS dependencies to the `platformio.ini` file:
     ```ini
     [env:esp32]
     platform = espressif32
     board = esp32dev
     framework = arduino
     lib_deps =
         micro-ROS/micro_ros_arduino
     ```

3. **Write the ESP32 Firmware**:
   - Configure your ESP32 to host a web server that listens for specific GPIO control HTTP requests (e.g., `/GPIO=1` or `/GPIO=2`) and toggles pins accordingly.
   - Example:
     ```cpp
     #include <WiFi.h>
     #include <micro_ros_arduino.h>

     const char* ssid = "Your_SSID";
     const char* password = "Your_PASSWORD";

     WiFiServer server(80);

     void setup() {
         WiFi.begin(ssid, password);
         while (WiFi.status() != WL_CONNECTED) {
             delay(1000);
         }
         server.begin();
     }

     void loop() {
         WiFiClient client = server.available();
         if (client) {
             String request = client.readStringUntil('\r');
             if (request.indexOf("/GPIO=1") != -1) {
                 digitalWrite(1, HIGH);
                 digitalWrite(2, HIGH);
             } else if (request.indexOf("/GPIO=2") != -1) {
                 digitalWrite(3, HIGH);
                 digitalWrite(4, HIGH);
             }
             client.flush();
         }
     }
     ```

4. **Upload Firmware**:
   - Connect your ESP32 to the PC and upload the code using PlatformIO.

5. **Note the ESP32 IP Address**:
   - After flashing, check the ESP32's IP address from the serial monitor or your router's admin page.

---

### Host PC Configuration

1. **Clone This Repository**:
   ```bash
   git clone https://github.com/AllaUjwalSai/Micro_ros_voice_control.git
   cd Micro_ros_voice_control
