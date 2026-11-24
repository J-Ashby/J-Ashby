# Battery Operation Guide - XIAO ESP32-S3 Sense

## Built-in Battery Features ⚡

The XIAO ESP32-S3 Sense includes:
- ✅ **Built-in LiPo charger chip** (SGM40567)
- ✅ **JST-PH 2.0mm battery connector pads**
- ✅ **100mA fast charging** via USB-C
- ✅ **Red LED charging indicator**
- ✅ **Automatic power switching** (USB/battery)
- ✅ **No external charging module needed!**

## Battery Specifications

### Recommended Battery
- **Voltage:** 3.7V single-cell LiPo
- **Connector:** JST-PH 2.0mm (2-pin)
- **Capacity:** 500-2500 mAh (depending on runtime needs)
- **Protection:** Built-in PCB protection circuit (recommended)

### Battery Pads Location
Located on the **bottom/back** of the XIAO board:
```
        [USB-C Port]
             ↑
    ┌────────────────┐
    │                │
    │  XIAO ESP32S3  │
    │     (back)     │
    │                │
    │  BAT-  BAT+    │ ← Solder pads
    │   •     •      │
    └────────────────┘
     ↑
  Nearest USB
```

**Important:** 
- **BAT-** (negative) is closest to USB-C port
- **BAT+** (positive) is farther from USB-C port

## Recommended Batteries (Amazon)

### Option 1: 1200mAh - Long Runtime (~6-8 hours) ⭐
**EEMB 1200mAh 3.7V LiPo**
- Capacity: 1200mAh
- Runtime: 6-8 hours continuous
- Size: ~60x34x6mm
- Price: ~$15-20
- Search: "1200mAh 3.7V JST-PH 2.0 battery"

### Option 2: 500-600mAh - Compact (~3-4 hours)
**EEMB 540mAh 3.7V LiPo**
- Capacity: 540mAh
- Runtime: 3-4 hours continuous
- Size: ~36x25x5mm (very compact!)
- Price: ~$10-15
- Recommended for portable builds

### Option 3: 2500mAh - Maximum Runtime (~12-15 hours)
**High capacity for all-day use**
- Capacity: 2500mAh
- Runtime: 12-15 hours continuous
- Size: ~65x50x8mm (larger)
- Price: ~$20-25
- Best for permanent installations

### Battery Search Terms (Amazon)
- "3.7V LiPo battery JST-PH 2.0"
- "Single cell lithium polymer battery JST connector"
- "1200mAh 3.7V rechargeable battery"

## Battery Installation

### What You'll Need
- 3.7V LiPo battery with JST-PH 2.0 male connector
- **OR** JST-PH 2.0 female connector pigtail to solder
- Soldering iron
- Solder
- Multimeter (to verify polarity!)

### Method 1: Pre-wired Battery (Easiest)
If your battery has a matching connector:
1. ⚠️ **VERIFY POLARITY with multimeter first!**
2. Many batteries have reversed polarity
3. Plug directly into XIAO battery pads (after soldering female connector)

### Method 2: Solder Female Connector (Recommended)

**Step 1: Get JST-PH 2.0 Connector**
- Buy "JST-PH 2.0mm female connector pigtail" (10-pack ~$5)
- Easier to swap batteries later
- Protects XIAO if battery fails

**Step 2: Prepare Connector**
```
Female JST Connector:
    ┌─────┐
    │ • • │ ← Socket
    └──┬──┘
       │
    Red│  │Black
       │  │
    (+ wire) (- wire)
```

**Step 3: Solder to XIAO**
1. Tin the wire ends
2. Tin the BAT+ and BAT- pads on XIAO
3. Solder **RED wire** to **BAT+** (farther from USB)
4. Solder **BLACK wire** to **BAT-** (closer to USB)
5. ⚠️ **Double-check polarity before connecting battery!**

**Step 4: Strain Relief**
- Use hot glue or electrical tape
- Secure wires to board to prevent breaking solder joints
- Leave some slack in wires

**Step 5: Verify**
1. Use multimeter to check polarity of battery
2. Red (positive) battery terminal should contact BAT+ wire
3. Black (negative) battery terminal should contact BAT- wire
4. ⚠️ **Reversed polarity can damage the board!**

### ⚠️ Polarity Warning

**CRITICAL:** Many batteries have reversed polarity!
```
Common Issue:
  Battery: Red = (+), Black = (-)
  But connector may be wired backwards!
  
Always verify with multimeter before connecting!
```

## Charging

### How to Charge
1. **Connect battery to XIAO**
2. **Plug USB-C cable** into XIAO
3. **Red LED will blink** while charging
4. **Red LED turns off** when fully charged
5. Charging time: 5-25 hours (depending on capacity)

### Charging Current
- **Fast charge:** 100mA
- **Trickle charge:** 0.9mA
- Charge time ≈ (Battery mAh / 100) hours
  - 500mAh: ~5 hours
  - 1200mAh: ~12 hours
  - 2500mAh: ~25 hours

### LED Charging Indicator

| LED State | Meaning |
|-----------|---------|
| **Blinking Red** | Charging in progress |
| **Off** | Fully charged OR no battery |
| **Brief on (30s)** | USB connected, no battery |

### Charging Tips
- Charge at room temperature (not cold/hot)
- Don't leave charging unattended for days
- Disconnect when fully charged (or leave connected - has overcharge protection)
- USB-C provides 5V for charging

## Power Consumption & Runtime

### dB Meter Power Draw

**Typical Operation:**
- XIAO ESP32-S3: ~40-50mA
- Display: ~40-80mA
- Microphone: ~2mA
- **Total: ~100-150mA average**

**Peak Operation:**
- Display refresh: ~150-200mA momentarily

### Runtime Calculations

| Battery | Capacity | Runtime (Est.) | Use Case |
|---------|----------|----------------|----------|
| Small | 500mAh | 3-4 hours | Portable, compact |
| Medium | 1200mAh | 6-8 hours | Daily use |
| Large | 2500mAh | 12-15 hours | All-day monitoring |
| Huge | 5000mAh | 24-30 hours | Multi-day |

**Formula:** Runtime (hours) ≈ (Battery mAh × 0.8) / 125mA

*0.8 factor accounts for inefficiency and voltage drop*

## Battery Monitoring

### Reading Battery Voltage

**Problem:** XIAO ESP32-S3 doesn't have dedicated battery voltage pin

**Solutions:**

#### Option 1: Use Two GPIO Pins (Voltage Divider)
Connect battery voltage through a voltage divider to an ADC pin:
```cpp
// Connect battery through 2:1 voltage divider to GPIO1
// 10kΩ resistor from BAT+ to GPIO1
// 10kΩ resistor from GPIO1 to GND

#define VBAT_PIN 1  // GPIO1 (A0)

float readBatteryVoltage() {
  int raw = analogRead(VBAT_PIN);
  float voltage = (raw / 4095.0) * 3.3 * 2.0; // Double due to divider
  return voltage;
}

float getBatteryPercent() {
  float voltage = readBatteryVoltage();
  // LiPo: 4.2V full, 3.3V empty
  if (voltage > 4.2) return 100.0;
  if (voltage < 3.3) return 0.0;
  return ((voltage - 3.3) / 0.9) * 100.0;
}
```

#### Option 2: Display Low Battery Warning (Simple)
No voltage monitoring, just warn when device acts sluggish

#### Option 3: External Fuel Gauge IC (Advanced)
Use MAX17048 or similar I2C fuel gauge chip

## Power Saving Features

### Reduce Display Brightness
Saves ~20-40mA:
```cpp
// In setup():
pinMode(6, OUTPUT);  // BL pin
analogWrite(6, 128);  // 50% brightness (0-255)
```

### Reduce Update Rate
Saves ~5-10mA:
```cpp
const int UPDATE_INTERVAL = 200; // 200ms instead of 100ms
```

### Deep Sleep Mode (Advanced)
For intermittent monitoring, can achieve **days** of runtime:

```cpp
#include "esp_sleep.h"

void goToDeepSleep(int seconds) {
  // Turn off display
  digitalWrite(6, LOW);  // Backlight off
  
  // Configure wakeup
  esp_sleep_enable_timer_wakeup(seconds * 1000000ULL);
  
  // Enter deep sleep
  Serial.println("Entering deep sleep...");
  esp_deep_sleep_start();
}

// Example: Measure for 5 seconds every minute
void loop() {
  // Take measurements for 5 seconds
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    // Normal operation
    currentDB = measureDB();
    smoothedDB = smoothedDB * 0.8 + currentDB * 0.2;
    updateNeedle(smoothedDB);
    displayDBValue(smoothedDB);
  }
  
  // Sleep for 55 seconds
  goToDeepSleep(55);
}

// Runtime: 500mAh battery = weeks!
```

## Battery Safety

### ⚠️ Important Safety Rules

1. **Never short circuit** battery terminals
2. **Don't puncture or crush** battery
3. **Charge at room temperature** (don't charge when cold/hot)
4. **Don't overcharge** (but charger has protection)
5. **Use protected batteries** (with PCB protection circuit)
6. **Store at 3.7-3.8V** if not using for weeks
7. **Dispose properly** at battery recycling center

### Signs of Bad Battery
- ⚠️ Swelling/puffing up
- ⚠️ Excessive heat during charge/use
- ⚠️ Rapid voltage drop
- ⚠️ Won't hold charge
- ⚠️ Physical damage

**Replace immediately if any of these occur!**

## Battery Enclosure Considerations

### Clearance
- Battery adds ~5-8mm thickness
- Plan enclosure to fit battery under/beside XIAO
- Keep battery accessible for replacement

### Ventilation
- LiPo batteries can off-gas (rarely)
- Don't seal completely airtight
- Small vent holes recommended

### Mounting
- Use double-sided foam tape
- Or 3D print battery holder
- Don't glue directly to battery!

### Strain Relief
- Secure wires with hot glue
- Don't let battery hang by wires
- Protect wires from sharp edges

## Portable Operation

### Fully Portable dB Meter
With battery installed:
1. ✅ No USB cable needed!
2. ✅ Runs 3-15 hours (depending on battery)
3. ✅ Pocket-sized
4. ✅ Perfect for field measurements
5. ✅ Charge via any USB-C cable when needed

### Use Cases
- **Field measurements** - Walk around measuring noise
- **Event monitoring** - Concerts, parties (cordless)
- **Vehicle noise** - Test car/motorcycle (portable)
- **Outdoor use** - Construction sites, nature
- **Wearable** - Clip to belt/pocket

## Hybrid Operation

### Best of Both Worlds
Battery remains connected when USB plugged in:

**USB plugged in:**
- Powers device from USB
- Charges battery simultaneously
- Display can show charging status

**USB unplugged:**
- Automatically switches to battery
- Seamless transition
- No interruption

## Code Additions for Battery

Add to your `main_xiao_s3.cpp`:

```cpp
// At top of file
#define VBAT_PIN 1    // If using voltage divider
#define BAT_WARNING_V 3.4  // Warn below 3.4V

// In setup()
pinMode(VBAT_PIN, INPUT);

// In loop() - check every 10 seconds
static unsigned long lastBatCheck = 0;
if (millis() - lastBatCheck > 10000) {
  float batV = readBatteryVoltage();
  Serial.print("Battery: ");
  Serial.print(batV, 2);
  Serial.println("V");
  
  if (batV < BAT_WARNING_V && batV > 2.0) {
    // Display warning
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("LOW BAT!", CENTER_X, 10);
  }
  
  lastBatCheck = millis();
}

float readBatteryVoltage() {
  // If using voltage divider on GPIO1
  int raw = analogRead(VBAT_PIN);
  return (raw / 4095.0) * 3.3 * 2.0;
}
```

## Shopping List for Battery Setup

**Required:**
1. ✅ XIAO ESP32-S3 Sense ($19)
2. ✅ 1.28" GC9A01 Display ($7)
3. ✅ 3.7V LiPo battery with JST-PH 2.0 ($10-20)

**Optional but Recommended:**
4. JST-PH 2.0 connector pigtails ($5 for 10-pack)
5. Multimeter ($10-15 if you don't have one)

**Total: ~$36-46** for fully portable dB meter!

## FAQs

**Q: Can I use the dB meter while charging?**
A: Yes! Use it plugged in while battery charges.

**Q: How long to charge?**
A: (Battery mAh / 100) hours. 1200mAh = ~12 hours.

**Q: Can I use any 3.7V battery?**
A: Yes, as long as it's a single-cell LiPo with JST-PH 2.0 connector (or you solder one).

**Q: What if polarity is wrong?**
A: Can damage the board! Always verify with multimeter first.

**Q: Can I charge from power bank?**
A: Yes! Plug USB-C from power bank to XIAO.

**Q: Battery drains fast?**
A: Reduce display brightness, update rate, or use deep sleep mode.

**Q: Can I replace battery?**
A: Yes! Use JST connector instead of soldering directly.

**Q: Max battery size?**
A: Any capacity, but physically must fit in your enclosure.

**Q: Will it auto-switch between USB and battery?**
A: Yes! Automatic and seamless.

**Q: Safe to leave battery connected when using USB?**
A: Yes! Charges automatically and safely.

## Summary

✅ **XIAO ESP32-S3 Sense has built-in LiPo charging**
✅ **No external module needed**
✅ **Just solder battery connector pads**
✅ **100mA charging via USB-C**
✅ **3-15 hour runtime** (depending on battery)
✅ **Fully portable dB meter!**

---

**Next Steps:**
1. Order XIAO ESP32-S3 Sense
2. Order display
3. Order 1200mAh LiPo battery
4. Order JST connector pigtails
5. Build your portable dB meter! 🔋📊
