# Display Buying Guide - 1.28" Round GC9A01

## Compatible Displays for ESP32 dB Meter

This project requires a **1.28" round TFT display** with the GC9A01 driver chip.

## Amazon Options (USA)

### 🏆 Recommended: DORHEA Display - $6.99
**Best value for money**

- **Product:** 1.28" LCD Round Screen TFT GC9A01
- **Price:** $5.99-$6.99
- **Link:** [Amazon Link](https://www.amazon.com/Screen-Display-240x240-Interface-Arduino/dp/B0GR44PSBL)
- **ASIN:** B0GR44PSBL
- **Reviews:** 4.4/5 stars (11 reviews)
- **Popularity:** 50+ bought in past month
- **Shipping:** Amazon Prime available

**Specifications:**
- Resolution: 240x240 pixels
- Driver: GC9A01 IC
- Interface: 4-wire SPI
- Voltage: 3-5V compatible
- Size: 45.5x48x11.5mm
- Viewing angle: Full angle IPS
- Brightness: High brightness backlight

### Alternative: MakerCircuit Circular Board - $9.99
**Premium option with circular PCB**

- **Product:** 1.28" Round TFT LCD IPS Display
- **Price:** $9.99 ($7.99 with Prime)
- **Link:** [Amazon Link](https://www.amazon.com/Round-Display-GC9A01-Circular-Square/dp/B0H7HSCY6V)
- **ASIN:** B0H7HSCY6V
- **Reviews:** 5.0/5 stars
- **Special Feature:** Circular PCB board (looks cleaner)

**Specifications:**
- Resolution: 240x240 pixels
- Driver: GC9A01
- Interface: 7-pin SPI
- Voltage: 3-5V
- Panel size: 38x45.5x3.2mm
- Display area: 32.4x32.4mm
- Operating temp: -20°C to 70°C

### Budget Option: DORHEA Single Unit - $5.99
**Cheapest option**

- **Product:** 1.28" TFT LCD Module
- **Price:** $5.99
- **Link:** [Amazon Link](https://www.amazon.com/Display-Board1-28-240x240-Interface-Arduino/dp/B0GR3L7TYW)
- **ASIN:** B0GR3L7TYW

### Two-Pack: EC Buying - $14.99
**Get a spare or build two units**

- **Product:** 2Pcs 1.28" Round TFT Display
- **Price:** $14.99 (2 displays)
- **Link:** [Amazon Link](https://www.amazon.com/EC-Buying-Display-Projects-Embedded/dp/B0G25K22DG)
- **ASIN:** B0G25K22DG
- **Reviews:** 4.2/5 stars

## AliExpress Options (Slower shipping, cheaper)

### Ultra-Budget: $3-5
Search terms: "1.28 inch GC9A01 round display"
- Usually $3-5 per display
- 2-4 week shipping from China
- Same quality as Amazon versions
- Look for 240x240 resolution

## Required Specifications

When buying ANY display for this project, verify:

| Specification | Required Value |
|---------------|----------------|
| **Size** | 1.28 inches (diagonal) |
| **Shape** | Round/Circular |
| **Resolution** | 240x240 pixels |
| **Driver Chip** | GC9A01 |
| **Interface** | SPI (4-wire or 7-pin) |
| **Voltage** | 3.3V or 3-5V compatible |
| **Color** | RGB full color (65K colors) |

## Pin Configuration

Standard pinout (may vary slightly by vendor):

```
Pin | Function           | Required
----|--------------------|----------
VCC | Power (3.3V)      | Yes
GND | Ground            | Yes
SCL | SPI Clock         | Yes
SDA | SPI Data (MOSI)   | Yes
RES | Reset             | Yes
DC  | Data/Command      | Yes
CS  | Chip Select       | Yes
BL  | Backlight         | Optional*
```

*Some displays have backlight permanently on, others have it controllable.

## Common Display Variants

### Type 1: Square PCB with Round Screen
- Most common type
- PCB is rectangular (~45x48mm)
- Screen is circular in the middle
- Usually 7-8 pins

### Type 2: Circular PCB
- Premium look
- PCB matches screen shape
- Slightly more expensive
- Same 7-8 pins

### Type 3: Bare Screen + Breakout
- Rare, more complex
- Requires soldering
- Not recommended for beginners

## What's Included

Typical package contents:
- ✅ 1.28" round display module
- ✅ Pin headers (may need soldering)
- ❌ Jumper wires (buy separately)
- ❌ Case/enclosure (DIY or 3D print)

## Display Quality Check

When you receive your display, test:
1. **Dead pixels** - Look for always-on/off dots
2. **Color accuracy** - All colors should be vibrant
3. **Viewing angle** - Should be visible from sides
4. **Backlight** - Should be bright and even
5. **Touch response** - Smooth, no flickering

## Compatibility Matrix

| Board | Display | Compatible? | Notes |
|-------|---------|-------------|-------|
| ESP32 | GC9A01 1.28" | ✅ Yes | Use 3.3V |
| XIAO ESP32-S3 | GC9A01 1.28" | ✅ Yes | Use 3.3V |
| Arduino Uno | GC9A01 1.28" | ✅ Yes | Use 5V |
| Raspberry Pi | GC9A01 1.28" | ✅ Yes | Use 3.3V |

## Wiring Considerations

### For ESP32 / XIAO ESP32-S3
- **Voltage:** Use 3.3V pin (not 5V!)
- **Logic Level:** 3.3V (native)
- **SPI Speed:** Up to 40MHz

### For Arduino 5V boards
- **Voltage:** Use 5V pin
- **Logic Level:** 5V
- **May need level shifters** for some displays

## Troubleshooting Display Issues

### Display doesn't turn on:
- Check VCC connected to 3.3V (not 5V on ESP32)
- Verify GND connection
- Check USB power is sufficient

### White screen only:
- Reset pin may be floating
- Try connecting RES to 3.3V briefly
- Check CS and DC pins

### Garbage/random colors:
- SPI wiring wrong (swap SCL/SDA?)
- Wrong driver in code (must be GC9A01)
- SPI speed too high

### Flickering:
- Poor power supply
- Loose wiring
- Add capacitor across VCC/GND

## Alternatives (NOT Recommended)

These will **NOT** work with this project without code changes:

❌ **ST7789 displays** - Different driver
❌ **ILI9341 displays** - Different driver, usually rectangular
❌ **OLED displays** - Completely different technology
❌ **1.3" displays** - Different size
❌ **Square displays** - Wrong aspect ratio for gauge

## Money-Saving Tips

1. **Buy from AliExpress** - Save 50% but wait 2-4 weeks
2. **Buy 2-pack** - Cheaper per unit, plus spare
3. **Watch for sales** - Black Friday, Prime Day
4. **Bundle with XIAO** - Some sellers offer combos

## Quick Decision Guide

**Need it fast?**
→ Buy DORHEA from Amazon ($6.99 + Prime)

**Want best price?**
→ Buy from AliExpress ($3-5 + wait)

**Building multiple units?**
→ Buy 2-pack from Amazon ($14.99)

**Want cleanest look?**
→ Buy MakerCircuit circular PCB ($9.99)

**On tight budget?**
→ Wait for AliExpress sale ($3)

## Final Recommendation

**For most people:**
Buy the **DORHEA 1.28" GC9A01 display** from Amazon for **$6.99**

**Why?**
- ✅ Affordable
- ✅ Fast shipping (Prime)
- ✅ Good reviews
- ✅ Popular choice (proven to work)
- ✅ Returns if defective

---

## Ready to Order?

1. Order XIAO ESP32-S3 Sense ($19)
2. Order 1.28" GC9A01 display ($7)
3. Get USB-C cable if needed ($5)
4. Get jumper wires ($5)

**Total: ~$36 for complete project!**

---

**Questions about displays? Check:**
- Main project: [README.md](README.md)
- XIAO wiring: [WIRING_XIAO_S3.md](WIRING_XIAO_S3.md)
- Regular ESP32: [WIRING.md](WIRING.md)
