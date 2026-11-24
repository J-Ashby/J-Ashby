# ESP32 dB Meter - Wiring Guide

## Components
- ESP32 Development Board
- MAX4466 Electret Microphone Amplifier
- 1.28" Round Display (GC9A01 Driver, 240x240)

## Wiring Connections

### MAX4466 Microphone Module
```
MAX4466        ->  ESP32
---------          -----
VCC            ->  3.3V
GND            ->  GND
OUT            ->  GPIO34 (ADC1_CH6)
```

**Note:** GPIO34 is an input-only pin suitable for ADC reading. You can use any ADC1 channel (GPIO32-39).

### 1.28" Round Display (GC9A01)
```
Display        ->  ESP32
--------           -----
VCC            ->  3.3V or 5V (check your display)
GND            ->  GND
SCL/SCLK       ->  GPIO18
SDA/MOSI       ->  GPIO23
RES/RST        ->  GPIO4
DC             ->  GPIO2
CS             ->  GPIO15
BL/BLK         ->  GPIO27 (backlight)
```

**Note:** If your display has different pin requirements, adjust the pins in `platformio.ini` build_flags section.

## Pin Summary Table

| Component          | Pin Name | ESP32 GPIO |
|-------------------|----------|------------|
| MAX4466           | OUT      | GPIO34     |
| MAX4466           | VCC      | 3.3V       |
| MAX4466           | GND      | GND        |
| Display (GC9A01)  | SCL      | GPIO18     |
| Display (GC9A01)  | SDA      | GPIO23     |
| Display (GC9A01)  | RST      | GPIO4      |
| Display (GC9A01)  | DC       | GPIO2      |
| Display (GC9A01)  | CS       | GPIO15     |
| Display (GC9A01)  | BL       | GPIO27     |
| Display (GC9A01)  | VCC      | 3.3V/5V    |
| Display (GC9A01)  | GND      | GND        |

## MAX4466 Gain Adjustment

The MAX4466 has a potentiometer on the back for gain adjustment:
- **Counter-clockwise**: Minimum gain (25x)
- **Clockwise**: Maximum gain (125x)

**Recommendation:** Start with the potentiometer at mid-position and adjust based on your environment's noise levels.

## Power Requirements

**Primary Power: USB (Recommended)**
- Power via ESP32's micro-USB or USB-C port
- USB provides 5V, ESP32 regulates to 3.3V internally
- Total current draw: ~200-300mA (well within USB limits)
- Use a quality USB cable to avoid voltage drops

**USB Power Sources:**
- Computer USB port (5V @ 500mA+) ✓
- USB wall adapter/charger (5V @ 1A+) ✓
- USB power bank (portable operation) ✓
- Phone charger (5V output) ✓

**Note:** The device is completely powered through the ESP32's USB port - no external power supply needed!

## Calibration

After assembly, you may need to adjust the dB calculation in the code:
1. Measure sound with a known dB meter (smartphone app)
2. Adjust the offset value (currently `94.0`) in the `measureDB()` function
3. Test in various sound conditions (quiet room, conversation, music)

## Troubleshooting

### Display shows nothing:
- Check all wiring connections
- Verify power supply is adequate
- Ensure display type in `platformio.ini` matches your display

### Needle doesn't move:
- Check MAX4466 wiring to GPIO34
- Verify microphone gain adjustment
- Monitor Serial output for dB readings

### Erratic readings:
- Reduce MAX4466 gain
- Add smoothing in code (adjust smoothing factor)
- Check for electromagnetic interference

### Display flickers:
- Use shorter wires
- Add decoupling capacitors near power pins (0.1µF)
- Reduce SPI frequency in `platformio.ini`
