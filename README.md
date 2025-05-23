# ESP32 Chatbot

A real-time voice-interactive chatbot powered by ESP32, Gemini AI, Google Web Speech API, and dual OLED animations. Inspired by Iron Man's J.A.R.V.I.S, this project listens for a trigger word and responds intelligently with animated eye expressions.

## 🔥 Features
- 🎤 **Voice Activated**: Always listening for the trigger word `Jarvis`.
- 💬 **Real-Time Q&A**: Captures speech, converts to text using Google Web Speech API (STT), and gets responses from Gemini AI.
- 👀 **OLED Animation**: Dual OLED eyes with blinking animation that resume smoothly during AI processing.
- 🧠 **Smart Display**:
  - After trigger word: shows `Yes` and `Sir!` for 2s.
  - Displays user question and Gemini's response for 15s.
  - Auto-interrupts if trigger word is used again.
- ❌ **Exit Commands**: Recognizes `Thanks Buddy`, `exit`, or `bye` and displays `See you` and `Soon` before shutdown.
- 🛠️ **Error Handling**: Displays `Sorry, Say again!` when input is unclear and immediately retries.


## 🧱 Hardware Components

ESP32 Dev Board  - Main controller                      
OLED Displays x2 - 1.3-inch I2C SSD1306 (left + right)  
Microphone       - I2S - captures voice inputs          
Python Backend   - Handles STT, Gemini, serial I/O      
Breadborad
Jumper Wires


## ⚡Connections

-> Microphone (I2S) to esp32
| Microphone Pin   | ESP32 GPIO    |
|------------------|---------------|
| VCC              | 3.3 V         |
| GND              | GND           |
| SCK              | GPIO 14       |
| WS               | GPIO 15       |
| SD               | GPIO 32       |


-> OLEDs Display (I2C) to esp32
| OLED Pin         | ESP32 GPIO    |
|------------------|---------------|
| VCC              | 3.3 V         |
| GND              | GND           |
| SCL (OLED 1)     | GPIO 22       |
| SDA (OLED 1)     | GPIO 21       |
| SCL (OLED 2)     | GPIO 16       |
| SDA (OLED 2)     | GPIO 17       |


## 🧠 AI Models

-> Google Web Speech API (STT)
-> Gemini AI via API call


## 💻 Software Flow

1. Always listens for **"Jarvis"**
2. Displays **"Yes" + "Sir!"**
3. Listens for your **question**
4. Sends to **Gemini AI**
5. Shows Q & A on OLEDs
6. Returns to **idle eye blinking**
7. Handles **exit or unclear commands**

![image](https://github.com/user-attachments/assets/512311f1-6a42-4b72-baaa-aeb350554b47)


## 🔧 Installation

1. Clone the Repository
git clone https://github.com/Vignesh-D-31/esp32-chatbot.git
cd esp32-chatbot

2. Set Up the Python Environment
Make sure you have Python 3.8 or later installed. Then install the required dependencies:
pip install -r requirements.txt
✅ Ensure you have your GOOGLE_APPLICATION_CREDENTIALS and Gemini API key configured in your environment

3. Upload Arduino Code to ESP32
-> Open chatbot_stt_gemini.ino in the Arduino IDE.
-> Select your ESP32 board from Tools > Board.
-> Connect your ESP32 via USB and select the appropriate COM port.
-> Click Upload.

4. Connect Your Hardware
-> Connect the following components:
-> Two OLEDs via I2C (left and right eye)
-> Microphone for capturing user voice
Refer to the pin mapping in the README.md ⚡Connections section.

5. Run the Python Interface
python main.py

This script handles:
-> Trigger word detection
-> Speech-to-Text (Google Web Speech API)
-> Response generation (Gemini AI)
-> Sending text to ESP32 via Serial
ℹ️ Make sure the ESP32 is connected to your computer via USB and the correct COM port is set in the Python script.


## 📸 Screenshots

-> Idle State:

![image](https://github.com/user-attachments/assets/a855a60d-8a84-4aef-b265-7df71cb4a316)
![image](https://github.com/user-attachments/assets/12a3a697-6ce4-455a-8084-082ef448aea2)
![image](https://github.com/user-attachments/assets/085983cf-ffb9-48f5-95ed-c0ec8e6e8ecf)

-> When inputs are not given:

![image](https://github.com/user-attachments/assets/8c063b9d-54f9-4d89-a394-7a089e47943c)

-> When exit commands are given:

![image](https://github.com/user-attachments/assets/ef7e24eb-c2e8-46a0-b562-36edba70fa6e)

-> Sample Questions and Responses:

![image](https://github.com/user-attachments/assets/c1424a3a-03bc-43f7-831e-835ac252ff79)
![image](https://github.com/user-attachments/assets/2b9eedf5-4870-48a9-b8ba-5958a094bd98)
![image](https://github.com/user-attachments/assets/d8d32b5b-9943-4750-bf52-80dff6723127)


## 🎥 Demo Video
https://github.com/user-attachments/assets/78c86c98-6938-4936-842f-309ea1af1ad0

