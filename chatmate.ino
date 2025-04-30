#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

TwoWire I2C_OLED1 = TwoWire(0);  // Left OLED
TwoWire I2C_OLED2 = TwoWire(1);  // Right OLED

Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED1, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED2, OLED_RESET);

// Microphone setup
#define MIC_PIN A0
#define SAMPLE_WINDOW 50  // in ms
int sensitivityThreshold = 25;  // Lower = more sensitive

bool activeDisplay = false;
unsigned long displayStart = 0;
unsigned long displayDuration = 15000;
String qText = "", aText = "";

void setup() {
  Serial.begin(115200);
  I2C_OLED1.begin(21, 22);
  I2C_OLED2.begin(18, 19);

  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display1.clearDisplay(); display1.display();
  display2.clearDisplay(); display2.display();
}

void loop() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');

    if (msg.startsWith("TRIGGER:")) {
      displayTextCentered(display1, "Yes", 2);
      displayTextCentered(display2, "Sir!", 2);
      delay(2000);
    }
    else if (msg.startsWith("EXIT:")) {
      displayTextCentered(display1, "See You", 2);
      displayTextCentered(display2, "Soon!", 2);
      delay(3000);
    }
    else if (msg.startsWith("ERROR:")) {
      displayTextCentered(display1, "Sorry,", 2);
      displayTextCentered(display2, "Say again!", 2);
      delay(2000);
    }
    else if (msg.startsWith("QNA:")) {
      int sep = msg.indexOf('|');
      if (sep != -1) {
        qText = msg.substring(4, sep);
        aText = msg.substring(sep + 1);
        displayQuestionAnswer(qText, aText);
        activeDisplay = true;
        displayStart = millis();
      }
    }
  }

  if (activeDisplay && millis() - displayStart >= displayDuration) {
    activeDisplay = false;
    qText = ""; aText = "";
  }

  if (!activeDisplay) {
    checkMicrophone();
    blinkingAnimation();
  }
}

void displayTextCentered(Adafruit_SSD1306 &disp, String text, int size) {
  disp.clearDisplay();
  disp.setTextSize(size);
  disp.setTextColor(SSD1306_WHITE);
  int16_t x1, y1;
  uint16_t w, h;
  disp.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  disp.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
  disp.println(text);
  disp.display();
}

void displayQuestionAnswer(String q, String a) {
  display1.clearDisplay();
  display1.setTextSize(1);
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(0, 0);
  display1.println(q);
  display1.display();

  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(SSD1306_WHITE);
  display2.setCursor(0, 0);
  display2.println(a);
  display2.display();
}

void blinkingAnimation() {
  int eyeY = 20, eyeH = 30, eyeR = 6;
  drawSingleEye(eyeY, eyeH, eyeR); delay(100);
  for (int i = 0; i <= 15; i += 3) {
    drawSingleEye(eyeY + i, eyeH - i, eyeR);
    delay(40);
  }
  drawSingleEye(35, 5, 3); delay(100);
  for (int i = 15; i >= 0; i -= 3) {
    drawSingleEye(eyeY + i, eyeH - i, eyeR);
    delay(40);
  }
}

void drawSingleEye(int y, int h, int r) {
  display1.clearDisplay();
  display2.clearDisplay();
  int eyeWidth = 30;
  int centerX = (SCREEN_WIDTH - eyeWidth) / 2;

  display1.fillRoundRect(centerX, y, eyeWidth, h, r, WHITE);
  display2.fillRoundRect(centerX, y, eyeWidth, h, r, WHITE);

  display1.display();
  display2.display();
}

void checkMicrophone() {
  unsigned long startMillis = millis();
  int signalMax = 0;
  int signalMin = 1024;

  while (millis() - startMillis < SAMPLE_WINDOW) {
    int sample = analogRead(MIC_PIN);
    if (sample > signalMax) signalMax = sample;
    if (sample < signalMin) signalMin = sample;
  }

  int peakToPeak = signalMax - signalMin;

  if (peakToPeak > sensitivityThreshold) {
    Serial.println("Sound Detected!");
    // Optional: Trigger animation or send feedback
  }
}
