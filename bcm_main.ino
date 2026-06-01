/*
 * ============================================================
 *   Automotive Body Control Module (BCM)
 *   Design and Implementation using Arduino Uno & TFT Display
 * ------------------------------------------------------------
 *   Author  : Mohammed Basith
 *   Reg. No : 311823106036
 *   Domain  : Embedded System and IoT Design
 * ============================================================
 *
 *  Pin Connections:
 *  ┌─────────────────────────────────────┐
 *  │ TFT ST7735                          │
 *  │   CS   → D10                        │
 *  │   RST  → D9                         │
 *  │   DC   → D8                         │
 *  │   MOSI → D11 (SPI)                  │
 *  │   SCK  → D13 (SPI)                  │
 *  │   VCC  → 3.3V / 5V                  │
 *  │   GND  → GND                        │
 *  ├─────────────────────────────────────┤
 *  │ LM35 Temperature Sensor             │
 *  │   OUT  → A0                         │
 *  │   VCC  → 5V                         │
 *  │   GND  → GND                        │
 *  ├─────────────────────────────────────┤
 *  │ Push Buttons (INPUT_PULLUP)         │
 *  │   Headlight  → D2                   │
 *  │   Left Ind.  → D3                   │
 *  │   Right Ind. → D4                   │
 *  ├─────────────────────────────────────┤
 *  │ Outputs                             │
 *  │   Headlight LED/Relay → D5          │
 *  │   Left Indicator LED  → D6          │
 *  │   Right Indicator LED → D7          │
 *  │   Buzzer              → D12         │
 *  └─────────────────────────────────────┘
 */

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// ─── TFT Pin Definitions ───────────────────────────────────
#define TFT_CS   10
#define TFT_RST   9
#define TFT_DC    8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ─── Input Pin Definitions ─────────────────────────────────
#define BTN_HEADLIGHT  2
#define BTN_LEFT_IND   3
#define BTN_RIGHT_IND  4

// ─── Output Pin Definitions ────────────────────────────────
#define OUT_HEADLIGHT  5
#define OUT_LEFT_IND   6
#define OUT_RIGHT_IND  7
#define OUT_BUZZER     12

// ─── Sensor Pin ────────────────────────────────────────────
#define LM35_PIN  A0

// ─── Temperature Threshold (°C) ────────────────────────────
#define TEMP_THRESHOLD  40.0

// ─── Blink Interval for Indicators (ms) ───────────────────
#define BLINK_INTERVAL  500

// ─── State Variables ───────────────────────────────────────
bool headlightState   = false;
bool leftIndState     = false;
bool rightIndState    = false;
bool faultActive      = false;

bool leftBlinkLED     = false;
bool rightBlinkLED    = false;

unsigned long lastBlinkTime = 0;
unsigned long lastTempTime  = 0;

float currentTemp = 0.0;

// ─── Button Debounce ───────────────────────────────────────
unsigned long lastDebounce[3] = {0, 0, 0};
#define DEBOUNCE_DELAY 200

// ──────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  // Input pins with internal pull-up
  pinMode(BTN_HEADLIGHT, INPUT_PULLUP);
  pinMode(BTN_LEFT_IND,  INPUT_PULLUP);
  pinMode(BTN_RIGHT_IND, INPUT_PULLUP);

  // Output pins
  pinMode(OUT_HEADLIGHT, OUTPUT);
  pinMode(OUT_LEFT_IND,  OUTPUT);
  pinMode(OUT_RIGHT_IND, OUTPUT);
  pinMode(OUT_BUZZER,    OUTPUT);

  // All outputs OFF initially
  digitalWrite(OUT_HEADLIGHT, LOW);
  digitalWrite(OUT_LEFT_IND,  LOW);
  digitalWrite(OUT_RIGHT_IND, LOW);
  digitalWrite(OUT_BUZZER,    LOW);

  // TFT Initialization
  tft.initR(INITR_BLACKTAB);   // Use INITR_GREENTAB if colors look wrong
  tft.setRotation(1);           // Landscape mode
  tft.fillScreen(ST77XX_BLACK);

  drawDashboardFrame();

  Serial.println("BCM System Initialized.");
}

// ──────────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  handleButtons(now);

  // Read temperature every 1 second
  if (now - lastTempTime >= 1000) {
    lastTempTime = now;
    currentTemp = readTemperature();
    checkFault();
    updateTempDisplay();
    Serial.print("Temperature: ");
    Serial.print(currentTemp);
    Serial.println(" °C");
  }

  // Blink indicators
  if (now - lastBlinkTime >= BLINK_INTERVAL) {
    lastBlinkTime = now;
    handleIndicatorBlink();
  }

  updateStatusDisplay();
}

// ──────────────────────────────────────────────────────────
//  Button Handling with Debounce (Toggle on press)
// ──────────────────────────────────────────────────────────
void handleButtons(unsigned long now) {
  // Headlight Button
  if (digitalRead(BTN_HEADLIGHT) == LOW && now - lastDebounce[0] > DEBOUNCE_DELAY) {
    lastDebounce[0] = now;
    headlightState = !headlightState;
    digitalWrite(OUT_HEADLIGHT, headlightState ? HIGH : LOW);
    Serial.println(headlightState ? "Headlight ON" : "Headlight OFF");
  }

  // Left Indicator Button
  if (digitalRead(BTN_LEFT_IND) == LOW && now - lastDebounce[1] > DEBOUNCE_DELAY) {
    lastDebounce[1] = now;
    leftIndState  = !leftIndState;
    rightIndState = false;  // Cancel right if left is pressed
    if (!leftIndState) digitalWrite(OUT_LEFT_IND, LOW);
    Serial.println(leftIndState ? "Left Indicator ON" : "Left Indicator OFF");
  }

  // Right Indicator Button
  if (digitalRead(BTN_RIGHT_IND) == LOW && now - lastDebounce[2] > DEBOUNCE_DELAY) {
    lastDebounce[2] = now;
    rightIndState = !rightIndState;
    leftIndState  = false;  // Cancel left if right is pressed
    if (!rightIndState) digitalWrite(OUT_RIGHT_IND, LOW);
    Serial.println(rightIndState ? "Right Indicator ON" : "Right Indicator OFF");
  }
}

// ──────────────────────────────────────────────────────────
//  Indicator Blink Logic
// ──────────────────────────────────────────────────────────
void handleIndicatorBlink() {
  if (leftIndState) {
    leftBlinkLED = !leftBlinkLED;
    digitalWrite(OUT_LEFT_IND, leftBlinkLED ? HIGH : LOW);
  } else {
    leftBlinkLED = false;
    digitalWrite(OUT_LEFT_IND, LOW);
  }

  if (rightIndState) {
    rightBlinkLED = !rightBlinkLED;
    digitalWrite(OUT_RIGHT_IND, rightBlinkLED ? HIGH : LOW);
  } else {
    rightBlinkLED = false;
    digitalWrite(OUT_RIGHT_IND, LOW);
  }
}

// ──────────────────────────────────────────────────────────
//  LM35 Temperature Reading
//  LM35: 10mV per °C, Vref = 5V, ADC = 10-bit (1024 steps)
// ──────────────────────────────────────────────────────────
float readTemperature() {
  int raw = analogRead(LM35_PIN);
  float voltage = (raw / 1024.0) * 5000.0;  // Convert to mV
  float tempC   = voltage / 10.0;            // LM35: 10mV per °C
  return tempC;
}

// ──────────────────────────────────────────────────────────
//  Fault Detection
// ──────────────────────────────────────────────────────────
void checkFault() {
  if (currentTemp >= TEMP_THRESHOLD) {
    if (!faultActive) {
      faultActive = true;
      digitalWrite(OUT_BUZZER, HIGH);
      drawFaultWarning();
      Serial.println("⚠ FAULT: High Temperature Detected!");
    }
  } else {
    if (faultActive) {
      faultActive = false;
      digitalWrite(OUT_BUZZER, LOW);
      clearFaultWarning();
      Serial.println("✓ Temperature Normal.");
    }
  }
}

// ──────────────────────────────────────────────────────────
//  TFT Dashboard Drawing
// ──────────────────────────────────────────────────────────
void drawDashboardFrame() {
  tft.fillScreen(ST77XX_BLACK);

  // Title bar
  tft.fillRect(0, 0, 160, 18, ST77XX_BLUE);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(20, 5);
  tft.print("AUTOMOTIVE BCM v1.0");

  // Divider lines
  tft.drawFastHLine(0, 20, 160, ST77XX_WHITE);
  tft.drawFastHLine(0, 80, 160, ST77XX_WHITE);

  // Section labels
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(2, 24);  tft.print("HEADLIGHT:");
  tft.setCursor(2, 38);  tft.print("LEFT IND :");
  tft.setCursor(2, 52);  tft.print("RIGHT IND:");
  tft.setCursor(2, 66);  tft.print("TEMP     :");

  // Footer
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(2, 84);
  tft.print("STATUS: NORMAL");
}

void updateStatusDisplay() {
  // Headlight status
  tft.fillRect(90, 24, 65, 10, ST77XX_BLACK);
  tft.setCursor(90, 24);
  if (headlightState) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("ON ");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.print("OFF");
  }

  // Left indicator status
  tft.fillRect(90, 38, 65, 10, ST77XX_BLACK);
  tft.setCursor(90, 38);
  if (leftIndState) {
    tft.setTextColor(ST77XX_GREEN);
    tft.print(leftBlinkLED ? "<<< " : "    ");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.print("OFF");
  }

  // Right indicator status
  tft.fillRect(90, 52, 65, 10, ST77XX_BLACK);
  tft.setCursor(90, 52);
  if (rightIndState) {
    tft.setTextColor(ST77XX_GREEN);
    tft.print(rightBlinkLED ? " >>>" : "    ");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.print("OFF");
  }
}

void updateTempDisplay() {
  tft.fillRect(90, 66, 65, 10, ST77XX_BLACK);
  tft.setCursor(90, 66);
  if (currentTemp >= TEMP_THRESHOLD) {
    tft.setTextColor(ST77XX_RED);
  } else {
    tft.setTextColor(ST77XX_GREEN);
  }
  tft.print(currentTemp, 1);
  tft.print(" C");
}

void drawFaultWarning() {
  tft.fillRect(0, 80, 160, 48, ST77XX_RED);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 84);
  tft.print("!! FAULT DETECTED !!");
  tft.setCursor(5, 96);
  tft.print("HIGH TEMP WARNING");
  tft.setCursor(5, 108);
  tft.print("CHECK ENGINE SYSTEM");
}

void clearFaultWarning() {
  tft.fillRect(0, 80, 160, 48, ST77XX_BLACK);
  tft.drawFastHLine(0, 80, 160, ST77XX_WHITE);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(2, 84);
  tft.print("STATUS: NORMAL");
}
