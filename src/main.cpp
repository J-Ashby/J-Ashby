#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// Pin definitions
#define MIC_PIN 34              // MAX4466 analog output to GPIO34 (ADC1_CH6)
#define SAMPLE_RATE 8000        // Sample rate in Hz
#define SAMPLE_WINDOW 50        // Sample window width in ms
#define MIN_DB 40.0             // Minimum dB for display (quiet)
#define MAX_DB 100.0            // Maximum dB for display (loud)

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

// Gauge angle settings (180 degree gauge, bottom half circle)
#define ANGLE_START 180
#define ANGLE_END 360
#define ANGLE_RANGE (ANGLE_END - ANGLE_START)

// Variables
float currentDB = 0;
float smoothedDB = 0;
float previousAngle = -1;
unsigned long lastUpdate = 0;
const int UPDATE_INTERVAL = 100; // Update display every 100ms

void setup() {
  Serial.begin(115200);
  
  // Initialize ADC
  analogReadResolution(12);  // 12-bit ADC (0-4095)
  analogSetAttenuation(ADC_11db);  // Full range: 0-3.3V
  
  // Initialize display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  // Draw static gauge elements
  drawGaugeBackground();
  
  Serial.println("ESP32 dB Meter Initialized");
}

void loop() {
  // Sample audio and calculate dB
  currentDB = measureDB();
  
  // Smooth the readings
  smoothedDB = smoothedDB * 0.8 + currentDB * 0.2;
  
  // Update display at defined interval
  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    updateNeedle(smoothedDB);
    displayDBValue(smoothedDB);
    lastUpdate = millis();
    
    // Debug output
    Serial.print("dB Level: ");
    Serial.println(smoothedDB, 1);
  }
}

float measureDB() {
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 4095;
  unsigned int sample;
  
  // Collect samples for SAMPLE_WINDOW ms
  while (millis() - startMillis < SAMPLE_WINDOW) {
    sample = analogRead(MIC_PIN);
    if (sample < 4095) {
      if (sample > signalMax) {
        signalMax = sample;
      }
      if (sample < signalMin) {
        signalMin = sample;
      }
    }
  }
  
  // Calculate peak-to-peak amplitude
  unsigned int peakToPeak = signalMax - signalMin;
  
  // Convert to voltage (3.3V reference, 12-bit ADC)
  float voltage = (peakToPeak * 3.3) / 4095.0;
  
  // Convert voltage to dB (approximate formula)
  // Adjust the offset (50) based on your environment
  float db = 20.0 * log10(voltage + 0.001) + 94.0;
  
  // Clamp to reasonable range
  if (db < MIN_DB) db = MIN_DB;
  if (db > MAX_DB) db = MAX_DB;
  
  return db;
}

void drawGaugeBackground() {
  // Draw title
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("dB METER", CENTER_X, 30);
  
  // Draw gauge arc segments with colors
  int arcWidth = 15;
  
  // Calculate angles for each zone
  float greenStartAngle = ANGLE_START;
  float greenEndAngle = ANGLE_START + (GREEN_END - GREEN_START) / (MAX_DB - MIN_DB) * ANGLE_RANGE;
  
  float yellowStartAngle = greenEndAngle;
  float yellowEndAngle = ANGLE_START + (YELLOW_END - YELLOW_START + (GREEN_END - GREEN_START)) / (MAX_DB - MIN_DB) * ANGLE_RANGE;
  
  float redStartAngle = yellowEndAngle;
  float redEndAngle = ANGLE_END;
  
  // Draw colored arc segments
  drawArcSegment(CENTER_X, CENTER_Y, GAUGE_RADIUS, arcWidth, greenStartAngle, greenEndAngle, TFT_GREEN);
  drawArcSegment(CENTER_X, CENTER_Y, GAUGE_RADIUS, arcWidth, yellowStartAngle, yellowEndAngle, TFT_YELLOW);
  drawArcSegment(CENTER_X, CENTER_Y, GAUGE_RADIUS, arcWidth, redStartAngle, redEndAngle, TFT_RED);
  
  // Draw tick marks
  drawTickMarks();
  
  // Draw center circle
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
  
  // Draw tick marks and labels
  for (float db = MIN_DB; db <= MAX_DB; db += 10) {
    float angle = ANGLE_START + ((db - MIN_DB) / (MAX_DB - MIN_DB)) * ANGLE_RANGE;
    float rad = angle * PI / 180.0;
    
    // Outer tick
    int x1 = CENTER_X + (GAUGE_RADIUS - 20) * cos(rad);
    int y1 = CENTER_Y + (GAUGE_RADIUS - 20) * sin(rad);
    int x2 = CENTER_X + (GAUGE_RADIUS - 30) * cos(rad);
    int y2 = CENTER_Y + (GAUGE_RADIUS - 30) * sin(rad);
    tft.drawLine(x1, y1, x2, y2, TFT_WHITE);
    
    // Label
    int labelX = CENTER_X + (GAUGE_RADIUS - 40) * cos(rad);
    int labelY = CENTER_Y + (GAUGE_RADIUS - 40) * sin(rad);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(String((int)db), labelX, labelY);
  }
}

void updateNeedle(float db) {
  // Calculate needle angle
  float angle = ANGLE_START + ((db - MIN_DB) / (MAX_DB - MIN_DB)) * ANGLE_RANGE;
  
  // Erase previous needle (if exists)
  if (previousAngle >= 0) {
    drawNeedle(previousAngle, TFT_BLACK);
  }
  
  // Draw new needle
  drawNeedle(angle, TFT_WHITE);
  
  previousAngle = angle;
}

void drawNeedle(float angle, uint16_t color) {
  float rad = angle * PI / 180.0;
  
  // Needle tip
  int tipX = CENTER_X + NEEDLE_LENGTH * cos(rad);
  int tipY = CENTER_Y + NEEDLE_LENGTH * sin(rad);
  
  // Needle base (wider for better visibility)
  float perpAngle1 = (angle + 90) * PI / 180.0;
  float perpAngle2 = (angle - 90) * PI / 180.0;
  int baseX1 = CENTER_X + 5 * cos(perpAngle1);
  int baseY1 = CENTER_Y + 5 * sin(perpAngle1);
  int baseX2 = CENTER_X + 5 * cos(perpAngle2);
  int baseY2 = CENTER_Y + 5 * sin(perpAngle2);
  
  // Draw needle as triangle
  tft.fillTriangle(tipX, tipY, baseX1, baseY1, baseX2, baseY2, color);
  
  // Redraw center circle on top
  tft.fillCircle(CENTER_X, CENTER_Y, 8, TFT_DARKGREY);
  tft.fillCircle(CENTER_X, CENTER_Y, 4, color);
}

void displayDBValue(float db) {
  // Clear previous value area
  tft.fillRect(CENTER_X - 40, CENTER_Y + 40, 80, 30, TFT_BLACK);
  
  // Display current dB value
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(MC_DATUM);
  
  char buffer[10];
  sprintf(buffer, "%.1f", db);
  tft.drawString(buffer, CENTER_X, CENTER_Y + 55);
  
  tft.setTextSize(1);
  tft.drawString("dB", CENTER_X, CENTER_Y + 75);
}
