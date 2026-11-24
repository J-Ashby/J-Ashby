#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <driver/i2s.h>
#include <ESP32Servo.h>

// I2S Microphone Configuration
#define I2S_WS 42
#define I2S_SD 41
#define I2S_SCK 43
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 16000
#define I2S_SAMPLE_BITS 16
#define I2S_READ_LEN (1024)
#define SAMPLE_WINDOW 50

// Servo Configuration
#define PAN_SERVO_PIN 1    // GPIO1 (D0)
#define TILT_SERVO_PIN 3   // GPIO3 (D2)
#define PAN_MIN 0
#define PAN_MAX 180
#define TILT_MIN 30
#define TILT_MAX 150

// dB Range settings
#define MIN_DB 40.0
#define MAX_DB 100.0

// Display settings
TFT_eSPI tft = TFT_eSPI();
#define CENTER_X 120
#define CENTER_Y 120
#define GAUGE_RADIUS 100
#define NEEDLE_LENGTH 85

// Color zones
#define GREEN_START 40.0
#define GREEN_END 60.0
#define YELLOW_START 60.0
#define YELLOW_END 80.0
#define RED_START 80.0
#define RED_END 100.0

// Gauge angle settings
#define ANGLE_START 180
#define ANGLE_END 360
#define ANGLE_RANGE (ANGLE_END - ANGLE_START)

// Servo objects
Servo panServo;
Servo tiltServo;

// Variables
float currentDB = 0;
float smoothedDB = 0;
float previousAngle = -1;
unsigned long lastUpdate = 0;
const int UPDATE_INTERVAL = 100;

// Servo positions
int panPos = 90;
int tiltPos = 90;

// Mode control
enum ScanMode { MANUAL, AUTO_SCAN, TRACK_SOUND };
ScanMode currentMode = MANUAL;
unsigned long scanStartTime = 0;

// I2S buffer
int16_t i2s_read_buff[I2S_READ_LEN];

// Function declarations
void setupI2S();
void setupServos();
float measureDB();
void drawGaugeBackground();
void drawArcSegment(int x, int y, int radius, int width, float startAngle, float endAngle, uint16_t color);
void drawTickMarks();
void updateNeedle(float db);
void drawNeedle(float angle, uint16_t color);
void displayDBValue(float db);
void displayServoPosition();
void setPan(int angle);
void setTilt(int angle);
void moveTo(int pan, int tilt, int speedMs = 15);
void autoScan();
void trackSound();

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("XIAO ESP32-S3 Pan/Tilt dB Meter Starting...");
  
  // Initialize I2S microphone
  setupI2S();
  
  // Initialize servos
  setupServos();
  
  // Initialize display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  // Draw static gauge elements
  drawGaugeBackground();
  
  Serial.println("Pan/Tilt dB Meter Initialized");
  Serial.println("Commands:");
  Serial.println("  'c' - Center position");
  Serial.println("  's' - Start auto scan");
  Serial.println("  't' - Track loudest sound");
  Serial.println("  'm' - Manual mode");
  Serial.println("  'l/r' - Pan left/right");
  Serial.println("  'u/d' - Tilt up/down");
}

void loop() {
  // Handle serial commands
  if (Serial.available()) {
    char cmd = Serial.read();
    switch(cmd) {
      case 'c': // Center
        currentMode = MANUAL;
        moveTo(90, 90, 10);
        Serial.println("Centered");
        break;
      case 's': // Auto scan
        currentMode = AUTO_SCAN;
        scanStartTime = millis();
        Serial.println("Starting auto scan...");
        break;
      case 't': // Track sound
        currentMode = TRACK_SOUND;
        Serial.println("Sound tracking mode");
        break;
      case 'm': // Manual
        currentMode = MANUAL;
        Serial.println("Manual mode");
        break;
      case 'l': // Pan left
        if (currentMode == MANUAL) setPan(panPos - 10);
        break;
      case 'r': // Pan right
        if (currentMode == MANUAL) setPan(panPos + 10);
        break;
      case 'u': // Tilt up
        if (currentMode == MANUAL) setTilt(tiltPos + 10);
        break;
      case 'd': // Tilt down
        if (currentMode == MANUAL) setTilt(tiltPos - 10);
        break;
    }
  }
  
  // Execute current mode
  switch(currentMode) {
    case MANUAL:
      // Sample audio and calculate dB
      currentDB = measureDB();
      smoothedDB = smoothedDB * 0.8 + currentDB * 0.2;
      
      // Update display
      if (millis() - lastUpdate > UPDATE_INTERVAL) {
        updateNeedle(smoothedDB);
        displayDBValue(smoothedDB);
        displayServoPosition();
        lastUpdate = millis();
        
        Serial.printf("dB: %.1f, Pan: %d°, Tilt: %d°\n", smoothedDB, panPos, tiltPos);
      }
      break;
      
    case AUTO_SCAN:
      autoScan();
      break;
      
    case TRACK_SOUND:
      trackSound();
      break;
  }
}

void setupI2S() {
  Serial.println("Setting up I2S microphone...");
  
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };
  
  esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed to install I2S driver: %d\n", err);
  }
  
  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("Failed to set I2S pins: %d\n", err);
  }
  
  Serial.println("I2S microphone initialized");
}

void setupServos() {
  Serial.println("Setting up servos...");
  
  // Allow allocation of all timers for servos
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  panServo.setPeriodHertz(50);
  tiltServo.setPeriodHertz(50);
  
  panServo.attach(PAN_SERVO_PIN, 500, 2500);
  tiltServo.attach(TILT_SERVO_PIN, 500, 2500);
  
  // Center position
  panServo.write(panPos);
  tiltServo.write(tiltPos);
  
  Serial.println("Servos initialized at center");
}

float measureDB() {
  size_t bytes_read = 0;
  int32_t sum = 0;
  int32_t min_val = INT32_MAX;
  int32_t max_val = INT32_MIN;
  
  unsigned long startMillis = millis();
  int samples_collected = 0;
  
  while (millis() - startMillis < SAMPLE_WINDOW) {
    esp_err_t result = i2s_read(I2S_PORT, &i2s_read_buff, I2S_READ_LEN * sizeof(int16_t), &bytes_read, portMAX_DELAY);
    
    if (result == ESP_OK && bytes_read > 0) {
      int samples_read = bytes_read / sizeof(int16_t);
      
      for (int i = 0; i < samples_read; i++) {
        int32_t sample = i2s_read_buff[i];
        
        if (sample > max_val) max_val = sample;
        if (sample < min_val) min_val = sample;
        
        sum += abs(sample);
        samples_collected++;
      }
    }
  }
  
  if (samples_collected == 0) {
    return MIN_DB;
  }
  
  int32_t peakToPeak = max_val - min_val;
  float amplitude_ratio = (float)peakToPeak / 32768.0;
  
  if (amplitude_ratio < 0.0001) amplitude_ratio = 0.0001;
  
  float db = 20.0 * log10(amplitude_ratio) + 94.0;
  
  if (db < MIN_DB) db = MIN_DB;
  if (db > MAX_DB) db = MAX_DB;
  
  return db;
}

void setPan(int angle) {
  panPos = constrain(angle, PAN_MIN, PAN_MAX);
  panServo.write(panPos);
}

void setTilt(int angle) {
  tiltPos = constrain(angle, TILT_MIN, TILT_MAX);
  tiltServo.write(tiltPos);
}

void moveTo(int pan, int tilt, int speedMs) {
  int targetPan = constrain(pan, PAN_MIN, PAN_MAX);
  int targetTilt = constrain(tilt, TILT_MIN, TILT_MAX);
  
  while (panPos != targetPan || tiltPos != targetTilt) {
    if (panPos < targetPan) panPos++;
    else if (panPos > targetPan) panPos--;
    
    if (tiltPos < targetTilt) tiltPos++;
    else if (tiltPos > targetTilt) tiltPos--;
    
    panServo.write(panPos);
    tiltServo.write(tiltPos);
    delay(speedMs);
  }
}

void autoScan() {
  static int scanPan = 0;
  static int scanTilt = 60;
  static bool scanDirection = true;
  
  setPan(scanPan);
  setTilt(scanTilt);
  delay(200);
  
  currentDB = measureDB();
  smoothedDB = smoothedDB * 0.8 + currentDB * 0.2;
  
  updateNeedle(smoothedDB);
  displayDBValue(smoothedDB);
  displayServoPosition();
  
  Serial.printf("Scan - Pan: %d°, Tilt: %d°, dB: %.1f\n", scanPan, scanTilt, smoothedDB);
  
  // Update scan position
  if (scanDirection) {
    scanPan += 10;
    if (scanPan >= 180) {
      scanDirection = false;
      scanTilt += 20;
      if (scanTilt > 120) {
        scanTilt = 60;
        currentMode = MANUAL;
        moveTo(90, 90);
        Serial.println("Scan complete!");
      }
    }
  } else {
    scanPan -= 10;
    if (scanPan <= 0) {
      scanDirection = true;
      scanTilt += 20;
      if (scanTilt > 120) {
        scanTilt = 60;
        currentMode = MANUAL;
        moveTo(90, 90);
        Serial.println("Scan complete!");
      }
    }
  }
}

void trackSound() {
  static unsigned long lastTrackTime = 0;
  
  if (millis() - lastTrackTime > 2000) {
    float maxDB = 0;
    int maxPan = panPos;
    int maxTilt = tiltPos;
    
    Serial.println("Tracking loudest sound...");
    
    // Quick scan around current position
    for (int p = panPos - 30; p <= panPos + 30; p += 15) {
      for (int t = tiltPos - 20; t <= tiltPos + 20; t += 20) {
        if (p >= PAN_MIN && p <= PAN_MAX && t >= TILT_MIN && t <= TILT_MAX) {
          setPan(p);
          setTilt(t);
          delay(150);
          
          float db = measureDB();
          
          if (db > maxDB) {
            maxDB = db;
            maxPan = p;
            maxTilt = t;
          }
        }
      }
    }
    
    Serial.printf("Tracking to Pan:%d° Tilt:%d° (%.1f dB)\n", maxPan, maxTilt, maxDB);
    moveTo(maxPan, maxTilt, 10);
    
    lastTrackTime = millis();
  }
  
  // Update display
  currentDB = measureDB();
  smoothedDB = smoothedDB * 0.8 + currentDB * 0.2;
  updateNeedle(smoothedDB);
  displayDBValue(smoothedDB);
  displayServoPosition();
}

void drawGaugeBackground() {
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("SCAN dB", CENTER_X, 30);
  
  int arcWidth = 15;
  
  float greenStartAngle = ANGLE_START;
  float greenEndAngle = ANGLE_START + (GREEN_END - GREEN_START) / (MAX_DB - MIN_DB) * ANGLE_RANGE;
  
  float yellowStartAngle = greenEndAngle;
  float yellowEndAngle = ANGLE_START + (YELLOW_END - YELLOW_START + (GREEN_END - GREEN_START)) / (MAX_DB - MIN_DB) * ANGLE_RANGE;
  
  float redStartAngle = yellowEndAngle;
  float redEndAngle = ANGLE_END;
  
  drawArcSegment(CENTER_X, CENTER_Y, GAUGE_RADIUS, arcWidth, greenStartAngle, greenEndAngle, TFT_GREEN);
  drawArcSegment(CENTER_X, CENTER_Y, GAUGE_RADIUS, arcWidth, yellowStartAngle, yellowEndAngle, TFT_YELLOW);
  drawArcSegment(CENTER_X, CENTER_Y, GAUGE_RADIUS, arcWidth, redStartAngle, redEndAngle, TFT_RED);
  
  drawTickMarks();
  
  tft.fillCircle(CENTER_X, CENTER_Y, 10, TFT_DARKGREY);
}

void drawArcSegment(int x, int y, int radius, int width, float startAngle, float endAngle, uint16_t color) {
  for (float angle = startAngle; angle < endAngle; angle += 1.0) {
    float rad = angle * PI / 180.0;
    int x1 = x + (radius - width) * cos(rad);
    int y1 = y + (radius - width) * sin(rad);
    int x2 = x + radius * cos(rad);
    int y2 = y + radius * sin(rad);
    tft.drawLine(x1, y1, x2, y2, color);
  }
}

void drawTickMarks() {
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  for (float db = MIN_DB; db <= MAX_DB; db += 10) {
    float angle = ANGLE_START + ((db - MIN_DB) / (MAX_DB - MIN_DB)) * ANGLE_RANGE;
    float rad = angle * PI / 180.0;
    
    int x1 = CENTER_X + (GAUGE_RADIUS - 20) * cos(rad);
    int y1 = CENTER_Y + (GAUGE_RADIUS - 20) * sin(rad);
    int x2 = CENTER_X + (GAUGE_RADIUS - 30) * cos(rad);
    int y2 = CENTER_Y + (GAUGE_RADIUS - 30) * sin(rad);
    tft.drawLine(x1, y1, x2, y2, TFT_WHITE);
    
    int labelX = CENTER_X + (GAUGE_RADIUS - 40) * cos(rad);
    int labelY = CENTER_Y + (GAUGE_RADIUS - 40) * sin(rad);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(String((int)db), labelX, labelY);
  }
}

void updateNeedle(float db) {
  float angle = ANGLE_START + ((db - MIN_DB) / (MAX_DB - MIN_DB)) * ANGLE_RANGE;
  
  if (previousAngle >= 0) {
    drawNeedle(previousAngle, TFT_BLACK);
  }
  
  drawNeedle(angle, TFT_WHITE);
  
  previousAngle = angle;
}

void drawNeedle(float angle, uint16_t color) {
  float rad = angle * PI / 180.0;
  
  int tipX = CENTER_X + NEEDLE_LENGTH * cos(rad);
  int tipY = CENTER_Y + NEEDLE_LENGTH * sin(rad);
  
  float perpAngle1 = (angle + 90) * PI / 180.0;
  float perpAngle2 = (angle - 90) * PI / 180.0;
  int baseX1 = CENTER_X + 5 * cos(perpAngle1);
  int baseY1 = CENTER_Y + 5 * sin(perpAngle1);
  int baseX2 = CENTER_X + 5 * cos(perpAngle2);
  int baseY2 = CENTER_Y + 5 * sin(perpAngle2);
  
  tft.fillTriangle(tipX, tipY, baseX1, baseY1, baseX2, baseY2, color);
  
  tft.fillCircle(CENTER_X, CENTER_Y, 8, TFT_DARKGREY);
  tft.fillCircle(CENTER_X, CENTER_Y, 4, color);
}

void displayDBValue(float db) {
  tft.fillRect(CENTER_X - 40, CENTER_Y + 40, 80, 30, TFT_BLACK);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(MC_DATUM);
  
  char buffer[10];
  sprintf(buffer, "%.1f", db);
  tft.drawString(buffer, CENTER_X, CENTER_Y + 55);
  
  tft.setTextSize(1);
  tft.drawString("dB", CENTER_X, CENTER_Y + 75);
}

void displayServoPosition() {
  tft.fillRect(0, 210, 240, 30, TFT_BLACK);
  
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  
  char buffer[30];
  sprintf(buffer, "P:%d T:%d", panPos, tiltPos);
  tft.drawString(buffer, 10, 215);
}
