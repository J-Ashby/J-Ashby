#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <driver/i2s.h>

// I2S Microphone Configuration (XIAO ESP32-S3 Sense built-in mic)
#define I2S_WS 42              // Word Select (LRCLK)
#define I2S_SD 41              // Serial Data (DOUT)
#define I2S_SCK 43             // Serial Clock (BCLK)
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 16000
#define I2S_SAMPLE_BITS 16
#define I2S_READ_LEN (1024)
#define SAMPLE_WINDOW 50       // Sample window width in ms

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

// Variables
float currentDB = 0;
float smoothedDB = 0;
float previousAngle = -1;
unsigned long lastUpdate = 0;
const int UPDATE_INTERVAL = 100;

// I2S buffer
int16_t i2s_read_buff[I2S_READ_LEN];

// Function declarations
void setupI2S();
float measureDB();
void drawGaugeBackground();
void drawArcSegment(int x, int y, int radius, int width, float startAngle, float endAngle, uint16_t color);
void drawTickMarks();
void updateNeedle(float db);
void drawNeedle(float angle, uint16_t color);
void displayDBValue(float db);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("XIAO ESP32-S3 Sense dB Meter Starting...");
  
  // Initialize I2S microphone
  setupI2S();
  
  // Initialize display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  // Draw static gauge elements
  drawGaugeBackground();
  
  Serial.println("XIAO ESP32-S3 dB Meter Initialized");
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

float measureDB() {
  size_t bytes_read = 0;
  int32_t sum = 0;
  int32_t min_val = INT32_MAX;
  int32_t max_val = INT32_MIN;
  
  unsigned long startMillis = millis();
  int samples_collected = 0;
  
  // Collect samples for SAMPLE_WINDOW ms
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
  
  // Calculate peak-to-peak amplitude
  int32_t peakToPeak = max_val - min_val;
  
  // Calculate average amplitude
  float average = (float)sum / samples_collected;
  
  // Convert to dB (empirical formula for I2S mic)
  // The 32768 is the max value for 16-bit signed audio
  float amplitude_ratio = (float)peakToPeak / 32768.0;
  
  // Avoid log of zero
  if (amplitude_ratio < 0.0001) amplitude_ratio = 0.0001;
  
  // Convert to dB scale (adjust offset as needed for calibration)
  float db = 20.0 * log10(amplitude_ratio) + 94.0;
  
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
  
  // Draw subtitle
  tft.setTextSize(1);
  tft.drawString("XIAO S3", CENTER_X, 50);
  
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
