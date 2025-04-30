#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create two Wire instances
TwoWire I2C_OLED1 = TwoWire(0);
TwoWire I2C_OLED2 = TwoWire(1);

// Initialize OLEDs on different buses
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED1, -1);  // Left eye
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED2, -1);  // Right eye

void setup() {
  Serial.begin(115200);

  // Start each I2C bus with corresponding pins
  I2C_OLED1.begin(21, 22);  // SDA, SCL for OLED1
  I2C_OLED2.begin(18, 19);  // SDA, SCL for OLED2

  // Initialize each display
  if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display 1 (Left Eye) not found!");
    while (true);
  }

  if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display 2 (Right Eye) not found!");
    while (true);
  }

  display1.clearDisplay(); display1.display();
  display2.clearDisplay(); display2.display();
  delay(1000);
}

void loop() {
  blinkingAnimation(false);  // Normal blink
  delay(1000);
  blinkingAnimation(true);   // Fast blink
  delay(1000);
}

void blinkingAnimation(bool fast) {
  int delayTime = fast ? 50 : 200;  // Faster eye-open timing

  drawEyesCentered(20, 30, 6);
  delay(delayTime);

  for (int i = 0; i <= 15; i += 5) {
    drawEyesCentered(20 + i, 30 - i, 6);
    delay(20);  // Faster movement
  }

  drawEyesCentered(35, 5, 3);
  delay(80);  // Short blink

  for (int i = 15; i >= 0; i -= 5) {
    drawEyesCentered(20 + i, 30 - i, 6);
    delay(20);  // Faster back
  }
}

void drawEyesCentered(int y, int height, int radius) {
  int eyeX = 49; // Centered for 128x64 screen with 30px width

  // Left eye on display1
  display1.clearDisplay();
  display1.fillRoundRect(eyeX, y, 30, height, radius, WHITE);
  display1.display();

  // Right eye on display2
  display2.clearDisplay();
  display2.fillRoundRect(eyeX, y, 30, height, radius, WHITE);
  display2.display();
}
