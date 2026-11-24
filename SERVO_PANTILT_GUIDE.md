# Pan/Tilt Servo Gantry - Optional Upgrade

## Overview

Add a robotic **pan/tilt platform** to your dB meter for:
- 🔄 **360° room scanning** - Survey entire space
- 🎯 **Sound source tracking** - Find loud areas automatically
- 📹 **Camera integration** - Use XIAO's built-in camera
- 🎮 **Remote control** - Pan/tilt via buttons or WiFi
- 🤖 **Autonomous sweep** - Pre-programmed patterns
- 📊 **Heatmap generation** - Create noise maps

## System Architecture

### Two-Part Design

```
┌─────────────────────────────────┐
│   TOP UNIT (Pan/Tilt Head)      │
│  ┌──────────────────────┐       │
│  │  XIAO ESP32-S3 Sense │       │
│  │  - Microphone        │       │
│  │  - Display (1.28")   │       │
│  │  - Camera (optional) │       │
│  │  - Battery           │       │
│  └──────────────────────┘       │
│         ↓ I2C or PWM             │
├─────────────────────────────────┤
│   BASE UNIT (Servo Platform)    │
│  ┌──────────────────────┐       │
│  │  Tilt Servo (SG90)   │       │
│  │  Pan Servo (SG90)    │       │
│  │  Optional: PCA9685   │       │
│  │  Power Supply        │       │
│  └──────────────────────┘       │
└─────────────────────────────────┘
```

## Hardware Components

### Required Parts

#### Servos (2x needed)
**Recommended: SG90 Micro Servo**
- **Voltage:** 4.8-6V (5V works great)
- **Torque:** 1.8 kg-cm
- **Speed:** 0.1 sec/60°
- **Weight:** 9g
- **Size:** 23×12×29mm
- **Price:** ~$3-5 each ($6-10 for 2)
- **Link:** [Amazon SG90 2-Pack](https://www.amazon.com/s?k=SG90+micro+servo+2+pack)

**Alternative: MG90S (Metal Gear)**
- Stronger, smoother
- ~$8-12 for 2
- Better for heavier loads

#### Servo Mounting Hardware
**Option 1: Direct Control (Simple)**
- Servos connect to XIAO GPIO pins
- 2 servos = 2 PWM pins
- Powered from 5V pin or external

**Option 2: Servo Driver Board (Better)**
- **PCA9685 16-Channel Servo Driver**
- I2C control (only 2 wires!)
- Independent 5V-6V power
- ~$8-12 on Amazon
- Link: [PCA9685 Servo Driver](https://www.amazon.com/s?k=PCA9685+servo+driver)

#### Pan/Tilt Bracket
**Option 1: 3D Print** (Recommended)
- Thingiverse has many designs
- Custom fit for your build
- Free (just filament cost)

**Option 2: Buy Pan/Tilt Kit**
- Pre-assembled brackets
- Includes servos sometimes
- ~$10-15 on Amazon
- Search: "SG90 pan tilt bracket"
- Link: [Pan/Tilt Bracket Kit](https://www.amazon.com/s?k=sg90+pan+tilt+bracket)

#### Power Supply
**For servos (5V, up to 2A):**
- USB power bank (easiest)
- 5V 2A power adapter
- Separate from XIAO (servos draw current spikes!)

### Optional Parts
- Push buttons (3x) for manual control
- Joystick module for smooth control
- Longer wires/cables
- Base weight/tripod mount
- Cable management clips

## Wiring Diagrams

### Option A: Direct Servo Control (Simple)

```
XIAO ESP32-S3 Sense (Top Unit)
┌─────────────────────┐
│  D2 (GPIO2)  ───────┼──→ Pan Servo Signal (Orange/Yellow)
│  D3 (GPIO3)  ───────┼──→ Tilt Servo Signal (Orange/Yellow)
│  5V          ───────┼──→ Servo VCC (Red) - both servos
│  GND         ───────┼──→ Servo GND (Brown) - both servos
└─────────────────────┘

Pan Servo (SG90)              Tilt Servo (SG90)
┌────────────┐                ┌────────────┐
│ Orange  ○──┼─GPIO2          │ Orange  ○──┼─GPIO3
│ Red     ○──┼─5V             │ Red     ○──┼─5V
│ Brown   ○──┼─GND            │ Brown   ○──┼─GND
└────────────┘                └────────────┘
```

**Pros:** Simple, no extra hardware
**Cons:** Limited current from XIAO, may need external 5V

### Option B: PCA9685 Servo Driver (Recommended)

```
XIAO ESP32-S3 (Top)           PCA9685 (Base)
┌─────────────┐               ┌──────────────┐
│ D4 (SDA) ───┼───────────────┼─→ SDA        │
│ D5 (SCL) ───┼───────────────┼─→ SCL        │
│ 3.3V     ───┼───────────────┼─→ VCC        │
│ GND      ───┼───────────────┼─→ GND        │
└─────────────┘               │              │
                              │ PWM 0 ───→ Pan Servo
                              │ PWM 1 ───→ Tilt Servo
                              │              │
  External 5V Power ──────────┼─→ V+         │
  (1-2A for servos)           │              │
└──────────────┘

**Servos connect to PCA9685 channels 0 & 1**
```

**Pros:** Clean I2C control, dedicated servo power, can add more servos later
**Cons:** Extra $10 cost, slightly more complex

### Power Architecture

```
USB Power Bank or 5V Adapter (2A+)
        │
        ├─→ XIAO ESP32-S3 (via USB-C)
        │    └─→ Display + Mic
        │
        └─→ PCA9685 or Servos (via V+ terminal)
             └─→ Pan + Tilt Servos
```

## Pin Assignments

### XIAO ESP32-S3 Sense Pins

| Pin | Function | Used For | Available? |
|-----|----------|----------|------------|
| D0 | GPIO1 | Available | ✅ Servo/Button |
| D1 | GPIO2 | Display DC | ❌ In use |
| D2 | GPIO3 | Available | ✅ Servo/Button |
| D3 | GPIO42 | I2S (Mic) | ❌ Internal |
| D4 | GPIO4 | Display RST | ❌ In use |
| D5 | GPIO5 | Display DC | ❌ In use |
| D6 | GPIO6 | Display BL | ❌ In use |
| D7 | GPIO7 | Display CS | ❌ In use |
| D8 | GPIO8 | Display SCL | ❌ In use |
| D9 | GPIO9 | Display MOSI | ❌ In use |
| D10 | GPIO43 | I2S (Mic) | ❌ Internal |
| SDA | GPIO6* | I2C Data | ⚠️ Used by display |
| SCL | GPIO7* | I2C Clock | ⚠️ Used by display |

**Solution:** Use different pins for servo control or software I2C

**Recommended:**
- **Pan Servo:** GPIO1 (D0) - PWM
- **Tilt Servo:** GPIO3 (D2) - PWM
- **Or use software I2C** on D0/D2 for PCA9685

## Code Implementation

### Basic Servo Control (Direct)

Add to `main_xiao_s3.cpp`:

```cpp
#include <ESP32Servo.h>

// Servo pins
#define PAN_SERVO_PIN 1   // GPIO1 (D0)
#define TILT_SERVO_PIN 3  // GPIO3 (D2)

// Servo objects
Servo panServo;
Servo tiltServo;

// Position limits (degrees)
#define PAN_MIN 0
#define PAN_MAX 180
#define TILT_MIN 30    // Don't flip backwards
#define TILT_MAX 150

// Current positions
int panPos = 90;   // Center
int tiltPos = 90;  // Level

void setupServos() {
  // Attach servos
  panServo.attach(PAN_SERVO_PIN);
  tiltServo.attach(TILT_SERVO_PIN);
  
  // Move to center position
  panServo.write(panPos);
  tiltServo.write(tiltPos);
  
  Serial.println("Servos initialized at center position");
}

void setPan(int angle) {
  panPos = constrain(angle, PAN_MIN, PAN_MAX);
  panServo.write(panPos);
}

void setTilt(int angle) {
  tiltPos = constrain(angle, TILT_MIN, TILT_MAX);
  tiltServo.write(tiltPos);
}

// Smooth movement
void moveTo(int pan, int tilt, int speedMs = 15) {
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
```

### PCA9685 Servo Driver (I2C)

```cpp
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create servo driver object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo channels
#define PAN_CHANNEL 0
#define TILT_CHANNEL 1

// Servo pulse lengths (calibrate for your servos)
#define SERVO_MIN 150  // Min pulse length (0°)
#define SERVO_MAX 600  // Max pulse length (180°)

void setupServos() {
  pwm.begin();
  pwm.setPWMFreq(50);  // Servo frequency: 50Hz
  
  // Center position
  setServoAngle(PAN_CHANNEL, 90);
  setServoAngle(TILT_CHANNEL, 90);
  
  Serial.println("PCA9685 servos initialized");
}

void setServoAngle(uint8_t channel, int angle) {
  angle = constrain(angle, 0, 180);
  int pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(channel, 0, pulse);
}

void setPan(int angle) {
  setServoAngle(PAN_CHANNEL, angle);
}

void setTilt(int angle) {
  setServoAngle(TILT_CHANNEL, angle);
}
```

### Auto-Scan Mode

```cpp
void scanRoom() {
  Serial.println("Starting room scan...");
  
  // Scan pattern: sweep horizontally at multiple heights
  for (int tilt = 60; tilt <= 120; tilt += 20) {
    setTilt(tilt);
    delay(500);
    
    // Sweep left to right
    for (int pan = 0; pan <= 180; pan += 10) {
      setPan(pan);
      delay(300);
      
      // Take measurement
      float db = measureDB();
      
      // Log position and dB level
      Serial.printf("Pan: %d°, Tilt: %d°, dB: %.1f\n", pan, tilt, db);
      
      // Could store in array for heatmap later
    }
    
    // Sweep right to left
    for (int pan = 180; pan >= 0; pan -= 10) {
      setPan(pan);
      delay(300);
      
      float db = measureDB();
      Serial.printf("Pan: %d°, Tilt: %d°, dB: %.1f\n", pan, tilt, db);
    }
  }
  
  // Return to center
  moveTo(90, 90);
  Serial.println("Scan complete!");
}
```

### Sound Source Tracking

```cpp
// Find direction of loudest sound
void trackLoudestSound() {
  float maxDB = 0;
  int maxPan = 90;
  int maxTilt = 90;
  
  // Quick scan
  for (int tilt = 60; tilt <= 120; tilt += 30) {
    setTilt(tilt);
    delay(200);
    
    for (int pan = 0; pan <= 180; pan += 20) {
      setPan(pan);
      delay(200);
      
      float db = measureDB();
      
      if (db > maxDB) {
        maxDB = db;
        maxPan = pan;
        maxTilt = tilt;
      }
    }
  }
  
  // Point at loudest source
  Serial.printf("Loudest sound: %.1f dB at Pan:%d° Tilt:%d°\n", 
                maxDB, maxPan, maxTilt);
  moveTo(maxPan, maxTilt, 10);
}
```

### Manual Control with Buttons

```cpp
#define BTN_LEFT 10   // GPIO10
#define BTN_RIGHT 44  // GPIO44
#define BTN_UP 11     // GPIO11
#define BTN_DOWN 12   // GPIO12
#define BTN_CENTER 13 // GPIO13

void setupButtons() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_CENTER, INPUT_PULLUP);
}

void handleButtons() {
  if (digitalRead(BTN_LEFT) == LOW) {
    setPan(panPos - 5);
    delay(50);
  }
  if (digitalRead(BTN_RIGHT) == LOW) {
    setPan(panPos + 5);
    delay(50);
  }
  if (digitalRead(BTN_UP) == LOW) {
    setTilt(tiltPos + 5);
    delay(50);
  }
  if (digitalRead(BTN_DOWN) == LOW) {
    setTilt(tiltPos - 5);
    delay(50);
  }
  if (digitalRead(BTN_CENTER) == LOW) {
    moveTo(90, 90);
    delay(200);
  }
}
```

## 3D Printing Files

### Recommended Designs on Thingiverse

**Search Terms:**
- "SG90 pan tilt bracket"
- "ESP32 pan tilt camera mount"
- "Mini pan tilt platform"

**Popular Designs:**
1. **Simple Pan/Tilt Platform**
   - Just servo brackets
   - Lightweight
   - Easy print (~2 hours)

2. **Camera Pan/Tilt Mount**
   - Designed for small cameras
   - Perfect for XIAO + display
   - Includes base plate

3. **Robotic Turret Base**
   - Heavy-duty
   - Cable management
   - Professional look

### DIY 3D Model Specifications

**Base Platform:**
- Dimensions: 80×80×20mm
- Mounting holes for pan servo
- Cable routing channels
- Weight pocket (optional)

**Tilt Bracket:**
- Servo horn attachment
- XIAO mounting plate (25×20mm)
- Display cutout (30mm circle)
- Battery compartment

**Files to Create:**
- `base_platform.stl`
- `pan_bracket.stl`
- `tilt_bracket.stl`
- `xiao_mount.stl`

### Print Settings
- **Material:** PLA or PETG
- **Layer Height:** 0.2mm
- **Infill:** 20-30%
- **Supports:** Yes (for overhangs)
- **Print Time:** 2-4 hours total

## Assembly Instructions

### Step 1: 3D Print Parts
1. Print base, pan bracket, tilt bracket
2. Clean up supports
3. Test fit servos

### Step 2: Install Pan Servo (Base)
1. Mount SG90 in base platform
2. Secure with M2 screws (usually included with servo)
3. Attach pan bracket to servo horn
4. Center servo (90°) before attaching

### Step 3: Install Tilt Servo
1. Mount servo to pan bracket
2. Attach tilt bracket to servo horn
3. Center servo before attaching

### Step 4: Mount XIAO Unit
1. Attach XIAO to tilt bracket
2. Route display cable
3. Secure battery (if using)
4. Add camera (optional)

### Step 5: Wiring
1. Run servo wires down to base
2. Connect to XIAO or PCA9685
3. Add power supply
4. Cable management ties

### Step 6: Calibration
1. Power on
2. Test pan servo (0-180°)
3. Test tilt servo (30-150°)
4. Adjust limits in code
5. Test smooth movement

## Control Modes

### Mode 1: Manual (Button Control)
- 4-way directional buttons
- Center button to reset
- Real-time dB display

### Mode 2: Auto-Scan
- Pre-programmed sweep pattern
- Logs dB at each position
- Generates position/dB map

### Mode 3: Sound Tracking
- Automatically points at loudest source
- Follows moving sound sources
- Good for finding noise culprits!

### Mode 4: WiFi Remote
- Control via web interface
- Phone/computer control
- Live video + dB feed

### Mode 5: Time-Lapse
- Programmed positions
- Take measurements over time
- Track sound changes

## Shopping List

### Minimum Setup (Direct Control)
1. SG90 Servos (2x) - $6-10
2. Pan/Tilt bracket or 3D print - $0-15
3. Jumper wires - $3
4. **Total: ~$10-30**

### Recommended Setup (PCA9685)
1. SG90 Servos (2x) - $6-10
2. PCA9685 Servo Driver - $8-12
3. Pan/Tilt bracket or 3D print - $0-15
4. 5V 2A power supply - $8
5. Buttons (5x) - $3
6. Jumper wires - $5
7. **Total: ~$30-55**

### Complete Kit
- Everything above PLUS:
- USB power bank (portable) - $15
- Joystick module - $5
- Project enclosure/base - $10
- **Total: ~$60-85**

## Amazon Links

### Servos
- [SG90 Micro Servo 2-Pack](https://www.amazon.com/s?k=SG90+micro+servo) - ~$7
- [MG90S Metal Gear](https://www.amazon.com/s?k=MG90S+servo) - ~$12

### Servo Driver
- [PCA9685 16-Channel Driver](https://www.amazon.com/s?k=PCA9685+servo+driver) - ~$10

### Pan/Tilt Hardware
- [SG90 Pan/Tilt Bracket Kit](https://www.amazon.com/s?k=sg90+pan+tilt+bracket) - ~$12

### Accessories
- [Push Button Kit](https://www.amazon.com/s?k=tactile+push+button+kit) - ~$8
- [Joystick Module](https://www.amazon.com/s?k=arduino+joystick+module) - ~$5

## Example Applications

### 1. Conference Room Monitor
- Scans room every 15 minutes
- Maps noise levels
- Identifies loud areas
- Logs data over time

### 2. Construction Site Safety
- Tracks loudest equipment
- Warns when >85dB
- Points at violator
- Portable with battery

### 3. Home Theater Calibration
- Scans listening positions
- Measures sound at each seat
- Generates heatmap
- Finds dead zones

### 4. Nature Recording
- Tracks bird calls
- Points camera + mic at source
- Records audio + video
- Wildlife monitoring

### 5. Stage/Concert Monitoring
- Monitors audience area
- Scans for feedback sources
- Real-time SPL mapping
- Points at problem areas

## Future Enhancements

- [ ] Add laser pointer to show measured direction
- [ ] GPS integration for outdoor mapping
- [ ] Multi-unit synchronized scanning
- [ ] AI sound classification (what type of noise?)
- [ ] Automatic obstacle avoidance
- [ ] Voice control ("scan left", "track sound")
- [ ] Integration with home automation
- [ ] Mobile app with AR visualization

## Summary

**Pan/Tilt upgrade adds:**
- ✅ 360° scanning capability
- ✅ Automated sound source location
- ✅ Remote directional control
- ✅ Room noise mapping
- ✅ Camera integration ready
- ✅ Professional appearance

**Cost:** $10-55 depending on features
**Complexity:** Moderate (requires 3D printing or bracket)
**Value:** Transforms simple meter into robotic scanner! 🤖

---

**Ready to build your robotic sound scanner?** Start with the basic servo setup and expand from there!
