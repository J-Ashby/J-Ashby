# Calibration Guide for ESP32 dB Meter

This guide will help you calibrate your dB meter for accurate readings.

## What You'll Need

1. Your assembled ESP32 dB Meter
2. A reference dB meter (options):
   - Professional dB meter (most accurate)
   - Smartphone app (e.g., "Sound Meter" on iOS/Android)
   - Another calibrated device
3. Various sound sources for testing
4. Serial monitor access to your ESP32

## Step-by-Step Calibration

### Step 1: Initial Setup

1. Upload the code to your ESP32
2. Connect to Serial Monitor (115200 baud)
3. Place your ESP32 meter and reference meter side-by-side
4. Point both microphones in the same direction

### Step 2: Test in Quiet Environment

1. Find a quiet room (bedroom, library)
2. Turn off fans, AC, and other noise sources
3. Wait 10 seconds for readings to stabilize
4. Record both readings:
   - ESP32 Serial Monitor: `________` dB
   - Reference meter: `________` dB
   - Difference: `________` dB

### Step 3: Test at Moderate Level

1. Play music at conversational volume or have a conversation
2. Wait for readings to stabilize
3. Record both readings:
   - ESP32 Serial Monitor: `________` dB
   - Reference meter: `________` dB
   - Difference: `________` dB

### Step 4: Test at Loud Level

1. Play loud music or use a louder sound source
2. Wait for readings to stabilize  
3. Record both readings:
   - ESP32 Serial Monitor: `________` dB
   - Reference meter: `________` dB
   - Difference: `________` dB

### Step 5: Calculate Average Offset

Average difference from all three tests:
```
Offset = (Diff1 + Diff2 + Diff3) / 3
Offset = (_____ + _____ + _____) / 3 = _____ dB
```

### Step 6: Adjust Code

1. Open `src/main.cpp`
2. Find the `measureDB()` function (around line 95)
3. Locate this line:
   ```cpp
   float db = 20.0 * log10(voltage + 0.001) + 94.0;
   ```
4. Adjust the offset value (94.0):
   - If ESP32 reads **too high**, **subtract** your offset
   - If ESP32 reads **too low**, **add** your offset
   
   Example: If average offset was +5 dB (ESP32 too high):
   ```cpp
   float db = 20.0 * log10(voltage + 0.001) + 89.0;  // 94 - 5
   ```

5. Re-upload the code

### Step 7: Verify Calibration

Repeat Steps 2-4 and verify the readings are now within ±2 dB of reference.

## Fine-Tuning MAX4466 Gain

The gain potentiometer on the MAX4466 affects sensitivity:

### If Readings Are Too Low Across All Levels:
1. Turn the potentiometer **clockwise** (increase gain)
2. Test again
3. Readjust code offset if needed

### If Readings Are Maxed Out or Clipping:
1. Turn the potentiometer **counter-clockwise** (decrease gain)
2. Test again
3. Readjust code offset if needed

### Recommended Gain Settings:

| Environment | Gain Setting | Potentiometer Position |
|-------------|--------------|------------------------|
| Quiet office | High | 75% clockwise |
| Normal use | Medium | 50% (middle) |
| Loud events | Low | 25% clockwise |

## Advanced: Frequency Response Compensation

The MAX4466 and ADC have frequency-dependent response. For better accuracy:

### Low Frequency (Bass) Compensation
If readings are low with bass-heavy sounds:
```cpp
// Add low-frequency boost factor
float db = 20.0 * log10(voltage + 0.001) + 94.0 + 2.0;
```

### High Frequency (Treble) Compensation
If readings are low with high-pitched sounds:
```cpp
// Add frequency-dependent weighting (requires FFT analysis)
// See advanced examples in project documentation
```

## Accuracy Expectations

With proper calibration, expect:
- **±3 dB**: Typical accuracy for hobby meters
- **±5 dB**: Acceptable for general monitoring
- **Professional meters**: ±1 dB or better (much more expensive)

## Common Issues

### Readings Jump Around
- **Solution**: Increase smoothing factor in code
  ```cpp
  smoothedDB = smoothedDB * 0.9 + currentDB * 0.1;  // More smoothing
  ```

### Readings Always at Minimum
- Check microphone wiring
- Increase MAX4466 gain
- Verify GPIO34 connection

### Readings Always at Maximum
- Decrease MAX4466 gain
- Check for electrical noise
- Move away from power supplies

### Readings Don't Match Reference
- Complete full calibration again
- Check reference meter is calibrated
- Try different sound levels
- Verify microphone placement

## Calibration Checklist

- [ ] Reference meter available
- [ ] Serial monitor connected
- [ ] Quiet environment tested
- [ ] Moderate sound tested
- [ ] Loud sound tested
- [ ] Offset calculated
- [ ] Code updated with new offset
- [ ] Re-tested and verified
- [ ] Gain adjusted if needed
- [ ] Final verification complete

## Environmental Factors

Be aware that these affect readings:
- **Temperature**: Affects electronics and sound propagation
- **Humidity**: Can affect microphone sensitivity
- **Air pressure**: Affects sound levels slightly
- **Microphone positioning**: Direction matters!
- **Reflections**: Walls and surfaces can amplify sound

## When to Re-Calibrate

Recalibrate if:
- Changed MAX4466 gain setting
- Moved to significantly different environment
- Modified hardware connections
- Readings seem consistently wrong
- After firmware updates

## Support

If calibration doesn't improve accuracy:
1. Check all wiring connections
2. Verify component specifications
3. Test with known good components
4. Review Serial output for anomalies
5. Post issue with calibration data for help

---

**Note**: This is a hobby-grade sound meter suitable for general monitoring. For safety-critical or regulatory compliance applications, use a professional calibrated meter.
