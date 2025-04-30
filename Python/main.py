import os
import serial
import time
import google.generativeai as genai
import speech_recognition as sr

# === CONFIGURATION ===
SERIAL_PORT = "COM4"  # Replace with your ESP32 port
BAUD_RATE = 115200
TRIGGER_WORD = "jarvis"
EXIT_COMMANDS = ["thanks buddy", "exit", "bye"]
API_KEY = "AIzaSyCaNacVuTL595r0b-Wu4jVkDRRGtUf3HWQ"  
MODEL_NAME = "gemini-1.5-flash"

# === SETUP ===
genai.configure(api_key=API_KEY)
esp32 = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
time.sleep(2)
recognizer = sr.Recognizer()
recognizer.energy_threshold = 250  # More sensitive to quieter voices
recognizer.pause_threshold = 0.8

print("Jarvis at your Service !")

def recognize_speech():
    with sr.Microphone() as source:
        print("🎙️ Listening...")
        recognizer.adjust_for_ambient_noise(source, duration=0.5)
        try:
            audio = recognizer.listen(source, timeout=5)
            text = recognizer.recognize_google(audio).lower()
            print("✅ Heard:", text)
            return text
        except sr.WaitTimeoutError:
            print("⏱️ Listening timeout.")
            return "UNKNOWN"
        except sr.UnknownValueError:
            print("❌ Could not understand audio.")
            return "UNKNOWN"
        except sr.RequestError as e:
            print("⚠️ Request error:", e)
            return "ERROR"

def get_gemini_response(prompt):
    try:
        model = genai.GenerativeModel(MODEL_NAME)
        response = model.generate_content(prompt)
        return response.text.strip().split("\n")[0]
    except Exception as e:
        print("💥 Gemini Error:", str(e))
        return "Sorry, I couldn't get that."

while True:
    try:
        text = recognize_speech()
        if text == "UNKNOWN" or text == "ERROR":
            continue

        if any(cmd in text for cmd in EXIT_COMMANDS):
            esp32.write("EXIT:\n".encode())
            esp32.flush() 
            print("👋 Exiting on command...")
            time.sleep(3)
            break

        if TRIGGER_WORD in text:
            esp32.write("TRIGGER:\n".encode())
            time.sleep(2)

            # Listen for the actual question
            while True:
                question = recognize_speech()
                if question == "UNKNOWN" or question == "ERROR":
                    esp32.write("ERROR:\n".encode())
                    continue

                if any(cmd in question for cmd in EXIT_COMMANDS):
                    esp32.write("EXIT:\n".encode())
                    esp32.flush()
                    print("👋 Bye !")
                    time.sleep(3)
                    exit()

                if TRIGGER_WORD in question:
                    esp32.write("TRIGGER:\n".encode())
                    time.sleep(2)
                    continue  # Restart question listening

                print("🧑 You:", question)
                answer = get_gemini_response(question)
                print("🤖 Jarvis:", answer)

                msg = f"QNA:{question}|{answer}\n"
                esp32.write(msg.encode())
                break

    except KeyboardInterrupt:
        print("\n🚪 Manual exit")
        break

esp32.close()
