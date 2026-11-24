# XIAO ESP32-S3 Sense dB Meter - Wiring Guide

## Components
- **Seeed Studio XIAO ESP32-S3 Sense** (with built-in microphone!)
- **1.28" Round Display** (GC9A01 Driver, 240x240)
- **USB-C cable** for power and programming

## Why XIAO ESP32-S3 Sense is Perfect for This Project

✅ **Built-in Digital Microphone** - No need for separate MAX4466 module!
✅ **Tiny Size** - 21mm x 17.8mm (thumb-sized)
✅ **More Powerful** - ESP32-S3 dual-core @ 240MHz
✅ **More Memory** - 8MB PSRAM + 8MB Flash
✅ **USB-C Power** - Modern connector
✅ **WiFi & BLE 5.0** - For future wireless features

## Simplified Wiring

Since the microphone is built into the XIAO board, you only need to connect the display!

### 1.28" Round Display (GC9A01) → XIAO ESP32-S3

```
Display Pin    →  XIAO ESP32-S3 Pin
-----------       ------------------
VCC            →  3V3 (3.3V power)
GND            →  GND (ground)
SCL/SCLK       →  D8  (GPIO8 - SPI Clock)
SDA/MOSI       →  D9  (GPIO9 - SPI MOSI)
RES/RST        →  D5  (GPIO5 - Reset)
DC             →  D4  (GPIO4 - Data/Command)
CS             →  D7  (GPIO7 - Chip Select)
BL/BLK         →  D6  (GPIO6 - Backlight)
```

## XIAO ESP32-S3 Pinout Reference

```
           ┌─────────────┐
           │   USB-C     │
           └─────────────┘
                 │││
           ┌─────────────┐
      D0 ──┤  1       14 ├── 5V (from USB)
      D1 ──┤  2       13 ├── GND
      D2 ──┤  3       12 ├── 3V3
      D3 ──┤  4       11 ├── D10
      D4 ──┤  5       10 ├── D9  (MOSI) → Display SDA
      D5 ──┤  6        9 ├── D8  (SCLK) → Display SCL
      D6 ──┤  7        8 ├── D7  (CS)   → Display CS
           └─────────────┘
                 ↑
            (Mic on back)
```

## Pin Connections Table

| Component | Display Pin | XIAO Pin | GPIO | Function |
|-----------|------------|----------|------|----------|
| Display | VCC | 3V3 | - | Power (3.3V) |
| Display | GND | GND | - | Ground |
| Display | SCL | D8 | GPIO8 | SPI Clock |
| Display | SDA | D9 | GPIO9 | SPI MOSI |
| Display | CS | D7 | GPIO7 | Chip Select |
| Display | DC | D4 | GPIO4 | Data/Command |
| Display | RST | D5 | GPIO5 | Reset |
| Display | BL | D6 | GPIO6 | Backlight |
| **Microphone** | **Built-in!** | **-** | **GPIO41-43** | **I2S Mic (internal)** |

## Built-in Microphone Details

The XIAO ESP32-S3 Sense has a **digital I2S microphone** on the back of the board:
- **Type:** MSM261S4030H0 or similar
- **Interface:** I2S (Inter-IC Sound)
- **Sample Rate:** Up to 16kHz for voice, 44.1kHz for audio
- **Pins Used (internally):**
  - GPIO42 - I2S WS (Word Select / LRCLK)
  - GPIO41 - I2S SD (Serial Data / DOUT)
  - GPIO43 - I2S SCK (Serial Clock / BCLK)

**You don't need to connect anything for the microphone - it's already wired internally!**

## Assembly Steps

### 1. Prepare Components
- XIAO ESP32-S3 Sense board
- 1.28" round display
- Breadboard or prototype board
- Jumper wires (7 wires needed)

### 2. Connect Display to XIAO
Follow the pinout table above. Use short wires for best signal integrity.

**Pro Tip:** Solder header pins to the XIAO for a secure breadboard connection.

### 3. Power via USB-C
Plug USB-C cable into XIAO - powers everything!

## Power Specifications

**USB-C Power:**
- Input: 5V from USB-C
- XIAO regulates to 3.3V
- Current draw: ~200-300mA total
- Well within USB 500mA limit

**Component Power:**
- XIAO ESP32-S3: ~100-150mA
- Display: ~40-100mA  
- Built-in Mic: ~1-2mA
- **Total: ~200mA typical**

## Comparison: XIAO S3 vs Regular ESP32

| Feature | Regular ESP32 | XIAO ESP32-S3 Sense |
|---------|---------------|---------------------|
| Size | ~25x50mm | 21x17.8mm 🏆 |
| Microphone | External (MAX4466) | Built-in 🏆 |
| Mic Type | Analog ADC | Digital I2S 🏆 |
| Wiring | 11 connections | 8 connections 🏆 |
| Memory | 520KB SRAM | 8MB PSRAM 🏆 |
| USB | Micro-USB | USB-C 🏆 |
| Cost | ~$8 + $5 mic | ~$19 total 🏆 |

## Breadboard Layout Suggestion

```
        XIAO ESP32-S3
         [USB-C Top]
    ┌────────────────┐
    │  D0  D1  D2  D3│
    │  D4  D5  D6  D7│
    │  D8  D9 D10    │
    │ GND 3V3  5V    │
    └────────────────┘
      │   │   │   │
      │   │   │   └─────→ Display CS (D7)
      │   │   └─────────→ Display BL (D6)
      │   └─────────────→ Display RST (D5)
      └─────────────────→ Display DC (D4)
      
    Display SCL ←── D8
    Display SDA ←── D9
    
    Display VCC ←── 3V3
    Display GND ←── GND
```

## Troubleshooting

### No Display Output
- ✓ Check all 8 wire connections
- ✓ Verify display is powered (3.3V)
- ✓ Try different USB-C cable
- ✓ Check display type is GC9A01

### No Microphone Readings
- ✓ Ensure you're using the XIAO **S3 Sense** version (has camera/mic)
- ✓ The regular XIAO S3 doesn't have a microphone!
- ✓ Mic is on the BACK of the board - don't block it
- ✓ Check Serial Monitor for I2S initialization messages

### Microphone Too Sensitive
- Adjust the dB offset in code (line ~137):
  ```cpp
  float db = 20.0 * log10(amplitude_ratio) + 94.0; // Try 90-100
  ```

### Display Flickers
- Use shorter wires
- Add 10µF capacitor between 3V3 and GND on XIAO
- Use better USB-C cable

### Programming Fails
- Hold BOOT button while connecting USB
- Press RESET button after upload
- Select correct board: "Seeed Studio XIAO ESP32S3"

## Physical Mounting Ideas

### Compact Handheld
- Mount XIAO on back of display
- 3D print case around both
- Microphone faces forward
- USB-C accessible on side

### Desktop Stand
- Mount display vertically
- XIAO hidden behind
- USB-C cable to computer/charger
- Mic unobstructed

### Wearable
- Both components very small!
- Could be worn on clothing
- Use short wires
- Battery bank in pocket

## Advantages of XIAO S3 Setup

1. **Simpler Wiring** - 8 connections vs 11
2. **Smaller Size** - Fits in tiny enclosures
3. **Digital Mic** - Better noise immunity than analog
4. **Higher Quality** - I2S audio is cleaner
5. **More Features** - Camera (future expansion), more memory
6. **Modern USB-C** - Reversible connector
7. **Same Cost** - $19 vs $8+$5 for separate components

## Next Steps

1. Wire up display following this guide
2. Upload code from `src/main_xiao_s3.cpp`
3. Use `platformio_xiao_s3.ini` for configuration
4. Calibrate as needed
5. Enjoy your ultra-compact dB meter!

---

**Note:** Make sure you have the **XIAO ESP32-S3 Sense** version (with camera and microphone). The regular XIAO ESP32-S3 (without "Sense") does not have the microphone module!
