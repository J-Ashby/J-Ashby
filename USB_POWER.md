# USB Power Guide for ESP32 dB Meter

## Overview

Your dB meter is **completely USB powered** - no external power supply, batteries, or additional wiring needed! Just plug in the USB cable and go.

## How It Works

```
USB Power Source (5V)
        │
        ├─→ ESP32 USB Port
        │
        ├─→ ESP32 Internal Regulator (3.3V)
        │
        ├─→ MAX4466 Microphone (3.3V)
        ├─→ GC9A01 Display (3.3V)
        └─→ All GPIO pins (3.3V logic)
```

## Power Consumption

| Component | Current Draw |
|-----------|--------------|
| ESP32 (WiFi off) | 80-160 mA |
| GC9A01 Display | 40-100 mA |
| MAX4466 Mic | ~3 mA |
| **Total** | **~200-300 mA** |

**USB Capability:** 500 mA (USB 2.0) to 900 mA (USB 3.0)
**Safety Margin:** ✓ Well within limits!

## USB Power Sources

### 1. Computer/Laptop USB Port ⭐ Recommended for Development
- **Pros:** Also provides serial communication for debugging
- **Voltage:** 5V
- **Current:** 500-900 mA
- **Best for:** Programming, testing, calibration

### 2. USB Wall Adapter ⭐ Recommended for Permanent Installation
- **Pros:** Stable power, no computer needed
- **Voltage:** 5V
- **Current:** 1A or higher (phone chargers work great!)
- **Best for:** Mounted installations, continuous monitoring
- **Example:** iPhone/Android phone charger

### 3. USB Power Bank ⭐ Recommended for Portable Use
- **Pros:** Battery powered, portable, no wall outlet needed
- **Voltage:** 5V
- **Current:** 1-2A typical
- **Runtime:** 10,000 mAh bank = ~20-30 hours continuous
- **Best for:** Field measurements, temporary setups
- **Note:** Some power banks auto-shutoff with low current - choose "always-on" models

### 4. USB Car Adapter
- **Pros:** Use in vehicle
- **Voltage:** 5V
- **Current:** 1-2A
- **Best for:** Car noise monitoring

## USB Cable Requirements

### Cable Type
- **Micro-USB** (most ESP32 boards) OR
- **USB-C** (newer ESP32 boards)
- Check your specific ESP32 board!

### Cable Quality Matters! ⚠️

**Good Cable:**
- Thick wire gauge (24 AWG or better)
- Short length (< 6 feet / 2 meters preferred)
- Data + Power wires (not charging-only)
- Minimal voltage drop

**Poor Cable Issues:**
- Thin wires cause voltage drop
- Display may flicker
- ESP32 may brown out/reset
- Erratic behavior

**Test:** If you experience issues, try a different USB cable first!

## Setup Options

### Option 1: Desktop Development Setup
```
[Computer USB Port] ───(USB Cable)─── [ESP32 Board]
                                           │
                                      [Breadboard]
                                      - Display
                                      - Microphone
```
- Leave plugged into computer
- Monitor Serial output
- Easy reprogramming
- Interactive calibration

### Option 2: Standalone/Wall Mounted
```
[Wall Outlet] ─── [USB Adapter] ───(USB Cable)─── [ESP32]
                    (5V 1A+)                          │
                                                 [Mounted]
                                                 - Display
                                                 - Microphone
```
- No computer needed
- Clean installation
- Permanent monitoring
- Use phone charger

### Option 3: Portable/Field Use
```
[USB Power Bank] ───(USB Cable)─── [ESP32 Board]
  (10,000mAh)                            │
                                    [Handheld]
                                    - Display
                                    - Microphone
```
- Battery powered
- Fully portable
- 20-30 hour runtime
- Mobile measurements

## Troubleshooting USB Power

### Display Not Turning On
- ✓ Check USB cable is fully inserted
- ✓ Try different USB cable
- ✓ Verify USB power source is working
- ✓ Check display wiring to ESP32 3.3V pin

### Display Flickering
- → Use shorter, thicker USB cable
- → Try different USB power source
- → Add 10µF capacitor across ESP32 power pins
- → Reduce display brightness in code

### ESP32 Keeps Resetting
- → Voltage drop from poor USB cable
- → Insufficient current from USB source
- → Try USB wall adapter (1A+)
- → Check for short circuits in wiring

### Power Bank Shuts Off
- → Some power banks detect "low current" and auto-shutoff
- → Look for power banks with "always-on" mode
- → Add a small load resistor (not usually needed)
- → Try different power bank

## Power Saving Tips

If you want to maximize battery life or reduce heat:

### 1. Reduce Display Brightness
```cpp
// In setup(), after tft.init():
analogWrite(27, 128);  // 50% brightness (0-255)
```

### 2. Lower Display Update Rate
```cpp
const int UPDATE_INTERVAL = 200; // 200ms instead of 100ms
```

### 3. Sleep Between Readings (Advanced)
```cpp
// Add light sleep between measurements
// Can extend battery life significantly
```

## Advanced: Current Measurement

Want to measure actual power consumption?

### Method 1: USB Power Meter
- Insert USB power meter between source and ESP32
- Displays voltage, current, power
- Cost: $10-20
- Example: "USB power meter tester"

### Method 2: Multimeter
- Set to mA mode (200-500 mA range)
- Interrupt USB power line
- Measure current flow
- **Caution:** Don't exceed meter rating!

## USB Power Specifications

| USB Standard | Voltage | Max Current | Power |
|--------------|---------|-------------|-------|
| USB 2.0 | 5V | 500 mA | 2.5W |
| USB 3.0 | 5V | 900 mA | 4.5W |
| USB 3.1 | 5V | 3A | 15W |
| USB-PD | 5-20V | Up to 5A | Up to 100W |

**Your dB meter:** Uses ~1.5W (well within all USB specs)

## Safety Notes

✓ **USB is safe** - Low voltage (5V)
✓ **No shock hazard** - Safe to handle while powered
✓ **No overload risk** - ESP32 draws well under USB limits
✓ **Hot plug safe** - Can connect/disconnect anytime

⚠ **Don't:**
- Use damaged USB cables
- Short circuit power pins
- Exceed ESP32 pin current limits (40 mA per GPIO)

## Quick Reference

**To Power On:**
1. Connect all wiring
2. Plug USB cable into ESP32
3. Plug other end into USB power source
4. Device starts immediately (no power switch)

**To Power Off:**
1. Unplug USB cable
(That's it!)

**Current Draw:** ~200-300 mA @ 5V
**Power:** ~1.5 W
**Runtime on 10,000 mAh bank:** ~20-30 hours

---

**Bottom Line:** Your ESP32 dB meter is designed to run entirely from USB power. Just plug it in and it works - no batteries, no wall warts, no complications! 🔌
