# ESP32 dB Meter - XIAO ESP32-S3 Sense Version

**Ultra-compact dB meter using the Seeed XIAO ESP32-S3 Sense with built-in microphone!**

![Version](https://img.shields.io/badge/Board-XIAO%20ESP32--S3%20Sense-green)
![Built-in Mic](https://img.shields.io/badge/Microphone-Built--in%20I2S-blue)

## Why Choose XIAO ESP32-S3 Sense?

✅ **Built-in Digital Microphone** - No external mic module needed!
✅ **Ultra-compact** - Just 21mm x 17.8mm (thumb-sized)
✅ **Simpler Build** - Only 8 wires instead of 11
✅ **Better Audio Quality** - Digital I2S microphone
✅ **More Powerful** - ESP32-S3 @ 240MHz with 8MB PSRAM
✅ **USB-C** - Modern, reversible connector
✅ **Same Price** - $19 vs $13 for separate components

## Hardware Requirements

### Essential Components
- **Seeed Studio XIAO ESP32-S3 Sense** (~$19) [Amazon Link](https://a.co/d/03qwjqYS)
  - ⚠️ Must be the "Sense" version with camera/microphone!
- **1.28" Round TFT Display** with GC9A01 driver (~$8-15)
- **USB-C cable** for programming and power
- Breadboard and 7 jumper wires

**Total Cost: ~$27-34** (vs $26-33 for regular ESP32 + MAX4466)

### What's Included in XIAO ESP32-S3 Sense
- ESP32-S3 dual-core processor (240MHz)
- 8MB PSRAM + 8MB Flash memory
- WiFi 2.4GHz + BLE 5.0
- **Built-in I2S digital microphone** 🎤
- OV2640 camera (for future projects)
- USB-C connector
- Ultra-compact 21x17.8mm form factor

## Quick Start

### 1. Get the Hardware
Order the XIAO ESP32-S3 **Sense** version (the one with camera/mic):
- [Amazon](https://a.co/d/03qwjqYS) - ~$19
- [Seeed Studio Official](https://www.seeedstudio.com/XIAO-ESP32S3-Sense-p-5639.html)

**Important:** Regular XIAO ESP32-S3 doesn't have a microphone!

### 2. Wire It Up
Only 8 connections needed! See [WIRING_XIAO_S3.md](WIRING_XIAO_S3.md)

**Quick Reference:**
```
Display → XIAO ESP32-S3
----------------------------
VCC  → 3V3    |  SCL → D8
GND  → GND    |  SDA → D9
DC   → D4     |  CS  → D7
RST  → D5     |  BL  → D6
```

Microphone: **Already built in!** (on back of board)

### 3. Install Software

**Using PlatformIO (Recommended):**
```bash
# Use the XIAO-specific config
cp platformio_xiao_s3.ini platformio.ini

# Upload
pio run -e seeed_xiao_esp32s3 --target upload
pio device monitor
```

**Using Arduino IDE:**
1. Install Seeed ESP32 boards:
   - Add URL to Board Manager: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
2. Select Board: **Seeed XIAO ESP32S3**
3. Open `src/main_xiao_s3.cpp`
4. Upload!

### 4. Test It
1. Plug in USB-C cable
2. Display shows gauge with colored zones
3. Make some noise and watch the needle move!
4. Check Serial Monitor for dB values

## Features

### Hardware Features
- 🎤 **Digital I2S Microphone** - Better quality than analog
- 📱 **1.28" Round Display** - Gauge with color zones
- 🔋 **USB-C Powered** - Computer, wall adapter, or power bank
- 🤏 **Tiny Size** - Fits anywhere
- 📶 **WiFi Ready** - For future wireless features

### Display Features
- 🟢 **Green Zone** - 40-60 dB (Quiet)
- 🟡 **Yellow Zone** - 60-80 dB (Moderate)
- 🔴 **Red Zone** - 80-100 dB (Loud)
- 🎯 **Smooth Needle** - Animated movement
- 🔢 **Digital Readout** - Exact dB value

### Software Features
- Real-time I2S audio sampling at 16kHz
- Peak-to-peak and RMS calculation
- Exponential smoothing for stable readings
- 10Hz display update rate
- Serial debugging output
- Configurable dB range and colors

## Technical Specifications

### XIAO ESP32-S3 Sense Specs
| Specification | Details |
|---------------|---------|
| Processor | ESP32-S3 dual-core @ 240MHz |
| Memory | 8MB PSRAM + 8MB Flash |
| Wireless | WiFi 2.4GHz, BLE 5.0 |
| Microphone | I2S digital MEMS mic |
| Camera | OV2640 (optional) |
| USB | USB-C (CDC, OTG) |
| Size | 21 x 17.8 x 11.5 mm |
| Power | 5V USB, 3.7V battery (JST) |
| Pins | 11 GPIO, I2C, SPI, UART |

### Microphone Specs
| Specification | Value |
|---------------|-------|
| Type | I2S MEMS (MSM261S4030H0 or similar) |
| Sample Rate | Up to 16kHz (voice) |
| Bit Depth | 16-bit |
| SNR | ~60dB |
| Sensitivity | -26dBFS |

### dB Meter Performance
| Metric | Value |
|--------|-------|
| Range | 40-100 dB |
| Update Rate | 10 Hz |
| Accuracy | ±3-5 dB (after calibration) |
| Noise Floor | ~40 dB |
| Response Time | ~100ms |

## Power Options

### 1. USB-C Computer/Laptop
- Plug directly into computer
- Provides serial debugging
- Always-on power

### 2. USB-C Wall Adapter
- Any phone charger (5V)
- Permanent installation
- ~200mA draw (safe)

### 3. USB-C Power Bank
- Portable operation
- 10,000mAh = 20-30 hours
- Field measurements

### 4. Battery (Advanced)
- JST connector on back of XIAO
- Use 3.7V LiPo battery
- Built-in charging circuit
- Fully portable!

## Code Structure

### Main Files for XIAO S3
```
platformio_xiao_s3.ini   # PlatformIO config for XIAO
src/main_xiao_s3.cpp     # Main code with I2S mic
WIRING_XIAO_S3.md        # XIAO-specific wiring guide
README_XIAO_S3.md        # This file
```

### Key Differences from Regular ESP32 Version
- Uses I2S microphone (not analog ADC)
- Different pin mappings for XIAO
- I2S driver initialization
- Digital audio processing

## Configuration

### Adjust dB Range
```cpp
#define MIN_DB 40.0   // Minimum
#define MAX_DB 100.0  // Maximum
```

### Adjust Color Zones
```cpp
#define GREEN_START 40.0
#define GREEN_END 60.0
#define YELLOW_START 60.0
#define YELLOW_END 80.0
#define RED_START 80.0
#define RED_END 100.0
```

### Calibration
Adjust offset in `measureDB()` function:
```cpp
float db = 20.0 * log10(amplitude_ratio) + 94.0;
//                                         ^^^^
//                                     Adjust this value
```

## Troubleshooting

### No Microphone Response
1. ✓ Verify you have XIAO ESP32-S3 **Sense** (not regular S3)
2. ✓ Microphone is on the BACK of the board
3. ✓ Don't cover the mic hole
4. ✓ Check Serial Monitor for I2S initialization

### Low Readings
- Increase offset value: `+ 94.0` → `+ 100.0`
- Speak/sound closer to board
- Mic is on back side

### High Readings
- Decrease offset value: `+ 94.0` → `+ 90.0`
- Move away from sound source
- Check for electrical noise

### Display Issues
- See [WIRING_XIAO_S3.md](WIRING_XIAO_S3.md) troubleshooting section
- Verify pin connections
- Check USB-C cable quality

## Comparison: XIAO S3 vs Regular ESP32

| Feature | Regular ESP32 + MAX4466 | XIAO ESP32-S3 Sense |
|---------|-------------------------|---------------------|
| **Size** | ~50x25mm + mic module | 21x17.8mm (all-in-one) ✅ |
| **Wiring** | 11 connections | 8 connections ✅ |
| **Microphone** | External analog | Built-in digital ✅ |
| **Audio Quality** | Good | Better (I2S) ✅ |
| **USB** | Micro-USB | USB-C ✅ |
| **Memory** | 520KB SRAM | 8MB PSRAM ✅ |
| **Cost** | ~$13 total | ~$19 total |
| **Enclosure** | Larger needed | Very compact ✅ |
| **Future Expand** | Limited | Camera, WiFi ready ✅ |

## Future Enhancements

- [ ] Data logging to SD card
- [ ] WiFi web interface for remote monitoring
- [ ] MQTT publishing to home automation
- [ ] Camera snapshot on loud events
- [ ] Frequency spectrum analyzer
- [ ] Battery operation with deep sleep
- [ ] Multiple unit networking
- [ ] Alert notifications

## Example Projects

### 1. Baby Monitor Sound Level
- Mount in nursery
- WiFi alerts on loud crying
- Battery powered

### 2. Office Noise Monitor
- Desk-mounted
- Display quiet zone compliance
- USB-C powered from monitor

### 3. Workshop Safety Meter
- Portable with battery
- Warn when ear protection needed
- Rugged enclosure

### 4. Home Recording Studio
- Monitor ambient noise
- Find quiet recording times
- Multiple units in different rooms

## Resources

### Hardware
- [XIAO ESP32-S3 Sense Wiki](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
- [Schematic PDF](https://files.seeedstudio.com/wiki/SeeedStudio-XIAO-ESP32S3/res/XIAO_ESP32S3_ExpBoard_v1.0_SCH.pdf)
- [3D Printable Cases](https://www.printables.com/search/models?q=xiao%20esp32)

### Software
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [I2S Audio Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/i2s.html)
- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)

### Community
- [Seeed Studio Forum](https://forum.seeedstudio.com/)
- [ESP32 Community](https://www.esp32.com/)

## License

This project is provided as-is for educational and hobby purposes.

## Support

For issues specific to XIAO ESP32-S3 Sense version:
1. Check [WIRING_XIAO_S3.md](WIRING_XIAO_S3.md)
2. Verify board is "Sense" version with microphone
3. Check Serial Monitor for error messages
4. Post issue with board version and error details

---

**Ready to build your ultra-compact dB meter? Get the XIAO ESP32-S3 Sense and start measuring! 🎤📊**
