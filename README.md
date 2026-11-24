# ESP32 dB Meter with Gauge Display

A real-time sound level meter using ESP32, MAX4466 microphone amplifier, and a 1.28" round display with an analog-style needle gauge.

![dB Meter](https://img.shields.io/badge/ESP32-Sound%20Meter-blue)

## Features

- **Real-time dB measurement** using MAX4466 electret microphone
- **Analog needle gauge** display with smooth animations
- **Color-coded zones**:
  - 🟢 Green: 40-60 dB (Quiet)
  - 🟡 Yellow: 60-80 dB (Moderate)
  - 🔴 Red: 80-100 dB (Loud)
- **Smooth needle movement** with exponential averaging
- **1.28" round display** (GC9A01 driver, 240x240 resolution)
- **Serial output** for debugging and data logging

## Hardware Requirements

- **ESP32 Development Board** (ESP32-WROOM-32 or similar)
- **MAX4466 Electret Microphone Amplifier Module**
- **1.28" Round TFT Display** with GC9A01 driver (240x240)
- Breadboard and jumper wires
- **USB cable** (Micro-USB or USB-C) - for programming AND power
- USB power source (computer, wall adapter, or power bank)

## Software Requirements

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- Libraries (auto-installed with PlatformIO):
  - TFT_eSPI
  - SPI

## Quick Start

### 1. Clone or Download

```bash
git clone <repository-url>
cd esp32-db-meter
```

### 2. Wiring

Follow the wiring guide in [WIRING.md](WIRING.md)

**Quick Reference:**
- MAX4466 OUT → GPIO34
- Display connections → See WIRING.md for full pinout
- **Power:** Just plug USB cable into ESP32 - see [USB_POWER.md](USB_POWER.md)

### 3. Upload Code

**Using PlatformIO:**
```bash
pio run --target upload
pio device monitor
```

**Using Arduino IDE:**
1. Open `src/main.cpp`
2. Install TFT_eSPI library
3. Configure TFT_eSPI for GC9A01 (see platformio.ini for settings)
4. Select ESP32 board and correct COM port
5. Upload

### 4. Calibration

1. Power on the device
2. Open Serial Monitor (115200 baud)
3. Compare readings with a reference dB meter
4. Adjust the offset in `measureDB()` function if needed:
   ```cpp
   float db = 20.0 * log10(voltage + 0.001) + 94.0;  // Adjust 94.0
   ```

## Display Layout

```
     ┌─────────────────┐
     │   dB METER      │
     │                 │
     │        🟢       │
     │      🟡 | 🔴    │
     │   ◀──────────▶  │
     │  40    70   100 │
     │                 │
     │      75.3       │
     │       dB        │
     └─────────────────┘
```

## Configuration

### Adjust dB Range
Edit in `main.cpp`:
```cpp
#define MIN_DB 40.0   // Minimum display value
#define MAX_DB 100.0  // Maximum display value
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

### Adjust Smoothing
Reduce value for faster response, increase for more stable readings:
```cpp
smoothedDB = smoothedDB * 0.8 + currentDB * 0.2;
//                        ^^^                ^^^
//                        old               new
```

### Display Pins
If your display uses different pins, modify `platformio.ini`:
```ini
-DTFT_MOSI=23
-DTFT_SCLK=18
-DTFT_CS=15
-DTFT_DC=2
-DTFT_RST=4
-DTFT_BL=27
```

## Understanding dB Levels

| dB Level | Description | Examples |
|----------|-------------|----------|
| 30-40 dB | Very Quiet | Library, whisper |
| 40-60 dB | Quiet | Conversation at home |
| 60-80 dB | Moderate | Office, traffic |
| 80-100 dB | Loud | Busy street, alarm clock |
| 100+ dB | Very Loud | Concert, power tools (ear protection!) |

## Troubleshooting

### No Display Output
- Check wiring connections
- Verify power supply (needs 5V @ >500mA)
- Ensure correct display driver (GC9A01)

### Needle Not Moving
- Check MAX4466 connection to GPIO34
- Adjust gain pot on MAX4466
- Monitor Serial output for readings

### Inaccurate Readings
- Calibrate using reference meter
- Adjust gain on MAX4466
- Modify offset in code
- Check for electrical noise

### Display Flickers
- Reduce SPI frequency
- Add 0.1µF capacitor near display power pins
- Use shorter wires

## Serial Output Example

```
ESP32 dB Meter Initialized
dB Level: 52.3
dB Level: 54.1
dB Level: 68.7
dB Level: 71.2
```

## Project Structure

```
esp32-db-meter/
├── platformio.ini      # PlatformIO configuration
├── src/
│   └── main.cpp        # Main application code
├── README.md           # This file
├── WIRING.md           # Detailed wiring guide
├── USB_POWER.md        # USB power guide (desktop/portable/wall)
├── CALIBRATION.md      # Step-by-step calibration
└── SCHEMATIC.txt       # ASCII connection diagram
```

## Future Enhancements

- [ ] Data logging to SD card
- [ ] WiFi connectivity for remote monitoring
- [ ] Peak hold indicator
- [ ] Adjustable time weighting (Fast/Slow)
- [ ] Frequency analysis
- [ ] MQTT publishing
- [ ] Web interface

## References

- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [MAX4466 Datasheet](https://datasheets.maximintegrated.com/en/ds/MAX4465-MAX4469.pdf)
- [GC9A01 Display Driver](https://github.com/Bodmer/TFT_eSPI)
- [Sound Level Reference](https://en.wikipedia.org/wiki/Sound_pressure#Examples_of_sound_pressure)

## License

This project is provided as-is for educational and hobby purposes.

## Contributing

Contributions welcome! Please open an issue or submit a pull request.
