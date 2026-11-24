# Visual Design & Rendering - Pan/Tilt dB Meter

## Complete System Overview

```
                    ╔═════════════════════════════╗
                    ║   XIAO ESP32-S3 SENSE      ║
                    ║   + 1.28" Round Display    ║
                    ╚═════════════════════════════╝
                              │││
                         ┌────┴┴┴────┐
                         │  Tilt Arm  │
                         └─────┬──────┘
                               │
                        ┌──────┴──────┐
                        │ Tilt Servo  │
                        │   (SG90)    │
                        └──────┬──────┘
                               │
                     ┌─────────┴─────────┐
                     │   Pan Platform    │
                     └─────────┬─────────┘
                               │
                        ┌──────┴──────┐
                        │  Pan Servo  │
                        │   (SG90)    │
                        └──────┬──────┘
                               │
                    ╔══════════╧══════════╗
                    ║    Base Platform    ║
                    ║  Battery/PCA9685    ║
                    ╚═════════════════════╝
```

## Front View - Complete Assembly

```
                         ┌─────────────┐
                         │  ┌───────┐  │
                         │  │ GAUGE │  │  ← 1.28" Display
                         │  │ 🟢🟡🔴 │  │     showing dB
                         │  └───────┘  │
                         └──────┬──────┘
                                │
                         ╔══════╧══════╗
                         ║    XIAO     ║  ← ESP32-S3 Sense
                         ║  ESP32-S3   ║     (21×18mm)
                         ║   [SENSE]   ║
                         ╚══════╤══════╝
                                │
                    ____________│____________
                   |                         |
                   |    Tilt Bracket        |  ← 3D printed
                   |___________ ___________|      or bracket kit
                              |│|
                         ┌────┴┴┴────┐
                         │  SG90     │       ← Tilt Servo
                         │  TILT     │          (up/down)
                         │ ◄───────► │
                         └─────┬─────┘
                               │
                    ___________│___________
                   |                       |
                   |   Pan Platform       |   ← Rotates 360°
                   |______________________|
                              │
                         ┌────┴────┐
                         │  SG90   │          ← Pan Servo
                         │  PAN    │             (left/right)
                         │ ◄─────► │
                         └────┬────┘
                              │
           ═══════════════════╧═══════════════════
          ║                                       ║
          ║         BASE PLATFORM                 ║
          ║  ┌──────────┐      ┌──────────┐     ║
          ║  │ PCA9685  │      │ Battery  │     ║
          ║  │  Servo   │      │  3.7V    │     ║
          ║  │ Driver   │      │ 1200mAh  │     ║
          ║  └──────────┘      └──────────┘     ║
          ║                                       ║
          ║           [USB-C Power Port]         ║
          ╚═══════════════════════════════════════╝
              Width: ~80mm    Depth: ~80mm
```

## Side View - Mechanical Detail

```
                    Display (front)
                         ┌──┐
                         │██│
                    ╔════╧══╧════╗
                    ║   XIAO     ║ 18mm height
                    ╚════╤══╤════╝
                         │  │ Wires
                    ═════╧══╧═════
                    │   Tilt    │
                    │  Bracket  │ 15mm
                    ═════════════
    90° ←              ┌───┐
    Tilt           ┌───┤███├───┐
    Range          │   └─┬─┘   │ SG90 Servo
                   │     │     │ 29mm × 12mm
                   └─────┴─────┘
                        ═══
                    ┌───────────┐
                    │    Pan    │
      360° ←        │ Platform  │ 10mm
      Rotation      └───────────┘
                        ┌───┐
                    ┌───┤███├───┐
                    │   └─┬─┘   │ SG90 Servo
                    │     │     │
                    └─────┴─────┘
                   ═══════════════
                  ║               ║
                  ║  Base (40mm) ║
                  ║  PCA + Batt  ║
                  ║               ║
                  ═══════════════
                  
Total Height: ~125-150mm (5-6 inches)
```

## Top View - Pan Rotation

```
                      FRONT
                        ↑
                        
            315°    0° / 360°    45°
               ╲       │       ╱
                ╲      │      ╱
                 ╲   ┌───┐   ╱
                  ╲  │ D │  ╱        D = Display
            270° ───┤ X ├─── 90°     X = XIAO
                  ╱  │ I │  ╲        S = Servos
                 ╱   └───┘   ╲
                ╱      │      ╲
               ╱       │       ╲
            225°      180°      135°
            
            ┌─────────────────┐
            │                 │
            │   Pan Servo     │  ← Rotates display
            │      ◉          │     assembly 360°
            │                 │
            └─────────────────┘
            
         ╔═══════════════════════╗
         ║                       ║
         ║  ┌─────┐   ┌──────┐  ║
         ║  │PCA  │   │ Bat  │  ║
         ║  │9685 │   │      │  ║
         ║  └─────┘   └──────┘  ║
         ║                       ║
         ╚═══════════════════════╝
              Base Platform
```

## Component Stack Diagram

```
Layer 5 (Top)     ┏━━━━━━━━━━━━━━━━━━━┓  
                  ┃   Round Display   ┃  40mm diameter
                  ┃   (GC9A01)        ┃  Gauge face
                  ┗━━━━━━━━━━━━━━━━━━━┛
                           ││
Layer 4           ┏━━━━━━━━╧╧━━━━━━━━┓
                  ┃  XIAO ESP32-S3    ┃  21 × 18mm
                  ┃  [◉][◉]  Mic ◉    ┃  Mic on back
                  ┃  Battery on back  ┃  Camera ready
                  ┗━━━━━━━━━━━━━━━━━━━┛
                           ││ Wires
Layer 3           ┌────────╧╧────────┐
                  │  Tilt Bracket     │  3D printed
                  │  ┌──────────────┐ │  or metal bracket
                  └──┴──────────────┴─┘
                           ││
Layer 2           ┌────────╧╧────────┐
                  │   SG90 Servo     │  Tilt axis
                  │   ┌───────┐      │  ± 60° range
                  │   │ ■■■■■ │      │
                  └───┴───────┴──────┘
                           ││
Layer 1           ╔════════╧╧════════╗
                  ║  Pan Platform     ║  Rotating disk
                  ║                   ║
                  ╠═══════════════════╣
                  ║   SG90 Servo     ║  Pan axis
                  ║   ┌───────┐      ║  360° rotation
                  ║   │ ■■■■■ │      ║
                  ╚═══╧═══════╧══════╝
                           ││
Layer 0 (Base)    ╔═══════════════════╗
                  ║  Base Platform    ║  80 × 80mm
                  ║                   ║
                  ║  ┌────┐  ┌─────┐ ║
                  ║  │PCA │  │Batt │ ║
                  ║  │9685│  │LiPo │ ║
                  ║  └────┘  └─────┘ ║
                  ║                   ║
                  ║  [USB-C]  [○○○]  ║  Power + Buttons
                  ╚═══════════════════╝
```

## Wiring Diagram - Complete System

```
┌─────────────────────────────────────────────────────────────┐
│                    XIAO ESP32-S3 SENSE                      │
│  ┌────────────────────────────────────────────────┐         │
│  │  GPIO1 (D0) ────────────────────────┐          │         │
│  │  GPIO3 (D2) ──────────────────┐     │          │         │
│  │  GPIO8 (SCL) ────────┐        │     │          │  USB-C  │
│  │  GPIO9 (SDA) ───┐    │        │     │          │   ││    │
│  │  GPIO4 (RST) ─┐ │    │        │     │          │   ││    │
│  │  GPIO2 (DC)  ─┼─┼─┐  │        │     │          │   ││    │
│  │  GPIO7 (CS)  ─┼─┼─┼──┼─┐      │     │          │   ││    │
│  │  GPIO6 (BL)  ─┼─┼─┼──┼─┼─┐    │     │          │   ││    │
│  │  3.3V ────────┼─┼─┼──┼─┼─┼────┼─────┼──────────┤   ││    │
│  │  GND ─────────┼─┼─┼──┼─┼─┼────┼─────┼──────────┤   ││    │
│  └───────────────┼─┼─┼──┼─┼─┼────┼─────┼──────────┘   ││    │
└──────────────────┼─┼─┼──┼─┼─┼────┼─────┼──────────────┼┼────┘
                   │ │ │  │ │ │    │     │              ││
         ┌─────────┘ │ │  │ │ │    │     │              ││
         │ ┌─────────┘ │  │ │ │    │     │       ┌──────┴┴─────┐
         │ │ ┌─────────┘  │ │ │    │     │       │ 5V @ 2A     │
         │ │ │ ┌──────────┘ │ │    │     │       │ Power Bank  │
         │ │ │ │ ┌──────────┘ │    │     │       │ or Adapter  │
         │ │ │ │ │ ┌──────────┘    │     │       └──────┬┬─────┘
         │ │ │ │ │ │                │     │              ││
         │ │ │ │ │ │  ┌─────────────┘     └──────┐       ││
         │ │ │ │ │ │  │                           │       ││
    ┌────▼─▼─▼─▼─▼─▼──▼───┐                 ┌────▼───────▼▼─────┐
    │ 1.28" GC9A01 Display │                 │   PCA9685 Servo    │
    │      ┌──────┐         │                 │   Driver Board     │
    │      │ GAUGE│         │                 │                    │
    │      │ 🟢🟡🔴│         │                 │  [0] Pan Servo ◄─┐ │
    │      └──────┘         │                 │  [1] Tilt Servo◄┐│ │
    └────────────────────────┘                 │                 ││ │
                                               │  VCC GND SDA SCL││ │
                                               └───┬──┬───┬───┬──┘│ │
                                                   │  │   │   │   │ │
                                              ┌────┘  │   │   │   │ │
                                              │   ┌───┘   │   │   │ │
                                              │   │  ┌────┘   │   │ │
                                              │   │  │  ┌─────┘   │ │
                                              │   │  │  │         │ │
                                           ┌──▼───▼──▼──▼─┐       │ │
                                           │   V+ GND     │       │ │
                                           └──┬───┬───────┘       │ │
                                              │   │               │ │
                      ┌───────────────────────┘   └───────┐       │ │
                      │                                   │       │ │
              ┌───────▼────────┐              ┌───────────▼───────▼─▼──┐
              │  Pan Servo     │              │  Tilt Servo             │
              │  SG90 (bottom) │              │  SG90 (middle)          │
              │  Orange Red Brn│              │  Orange Red Brown       │
              └────────────────┘              └─────────────────────────┘
                 (Rotates                        (Tilts display
                  base 360°)                      up/down ±60°)
```

## Movement Ranges

```
PAN SERVO (Horizontal Rotation)
═══════════════════════════════

         FRONT VIEW
              │
     315° ╱   │   ╲ 45°
         ╱    │    ╲
    ────┼────(●)────┼──── 90° (Full Right)
         ╲         ╱
      225°╲   │   ╱ 135°
              │
      180° (Full Back)

Range: 0° to 180° (can be extended to 360° with continuous servo)


TILT SERVO (Vertical Angle)
════════════════════════════

         150° Max Up
              │╱
              │
    ──────────●────────── 90° (Level)
              │
              │╲
         30° Max Down

Range: 30° to 150° (prevents flipping backward)
```

## Scan Pattern Visualization

```
AUTO-SCAN PATTERN (Room Mapping)
═════════════════════════════════

Top View - Horizontal Sweeps:
                                      
  SCAN HEIGHT 1 (60° Tilt - Lower)
  
  0°  ──→──→──→──→──→──→──→──→  180°
      🔊 Sample every 10°
  
  SCAN HEIGHT 2 (90° Tilt - Level)
  
  0°  ←─←─←─←─←─←─←─←─←─←─←─←─  180°
      🔊 Sample every 10°
  
  SCAN HEIGHT 3 (120° Tilt - Higher)
  
  0°  ──→──→──→──→──→──→──→──→  180°
      🔊 Sample every 10°


3D Coverage Pattern:
                    ╱│╲
                   ╱ │ ╲ 
              120°╱  │  ╲    ← High scan
                 ╱   │   ╲
            90°─────(●)───── ← Level scan  
                 ╲   │   ╱
              60° ╲  │  ╱    ← Low scan
                   ╲ │ ╱
                    ╲│╱

Result: 3 × 19 = 57 measurement points per scan
```

## Sound Tracking Mode

```
LOUDEST SOUND TRACKING
══════════════════════

Before Tracking:          After Tracking:
                                 
     dB Readings:            Points at loudest:
                                 
     45  48  52                   52  55  60
      •   •   •                    •   •   • 
        ╲ │ ╱                        ╲ │ ╱
    42 ──(●)── 50              52 ───(●)── 55
        ╱ │ ╲                        ╱ │ ╲
      •   •   •                    •   •   •
     40  43  47                   48  65  58
     
    Center position            Rotated to 65dB source!
                                     ↑
                              Tracks automatically
```

## Physical Dimensions

```
COMPACT PROFILE
═══════════════

Side View with Measurements:

              30mm
         ├──────────┤
         ┌──────────┐    ← Display (1.28" = 32mm)
         └─────┬────┘
               │ 18mm    ← XIAO board
         ╔═════╧═════╗
         ║   XIAO    ║
         ╚═════╤═════╝
               │ 15mm    ← Tilt bracket
         ┌─────┴─────┐
         │   Tilt    │
         └─────┬─────┘
               │ 30mm    ← Servo height
         ┌─────┴─────┐
         │  Servo    │
         └─────┬─────┘
               │ 10mm    ← Pan platform
         ┌─────┴─────┐
         │    Pan    │
         └─────┬─────┘
               │ 30mm    ← Pan servo
         ┌─────┴─────┐
         │  Servo    │
         └─────┬─────┘
               │ 40mm    ← Base
    ═════════════════════
    ║                  ║
    ║      BASE        ║
    ║                  ║
    ═════════════════════
         80mm × 80mm
         
Total Height: ~145mm (5.7 inches)
Footprint: 80 × 80mm (3.1 × 3.1 inches)
Weight: ~150-200g with battery
```

## Base Platform Layout (Top View)

```
╔═══════════════════════════════════════╗
║          80mm × 80mm Base            ║
║                                       ║
║   ┌──────────────┐   ┌────────────┐ ║
║   │   PCA9685    │   │  Battery   │ ║
║   │  Servo       │   │  1200mAh   │ ║
║   │  Driver      │   │  LiPo      │ ║
║   │              │   │  3.7V      │ ║
║   │  [I2C] [V+]  │   └────────────┘ ║
║   └──────────────┘                   ║
║                                       ║
║   ┌───┐  ┌───┐  ┌───┐  ┌───┐  ┌───┐║
║   │ ○ │  │ ○ │  │ ○ │  │ ○ │  │ ○ │║ Control
║   └───┘  └───┘  └───┘  └───┘  └───┘║ Buttons
║     ↑      ↑      ↑      ↑      ↑  ║
║   Left  Right   Up   Down  Center  ║
║                                       ║
║            [USB-C Power]              ║
║                                       ║
║   ┌─────────────────────────────┐   ║
║   │  Servo Mounting Hole        │   ║
║   │         (Center)             │   ║
║   └─────────────────────────────┘   ║
╚═══════════════════════════════════════╝
```

## Color-Coded Cable Management

```
CABLE COLOR SCHEME
══════════════════

Display Wiring:
  RED    → 3.3V Power
  BLACK  → Ground
  ORANGE → Clock (SCL)
  YELLOW → Data (SDA)
  GREEN  → Reset (RST)
  BLUE   → DC
  PURPLE → CS
  WHITE  → Backlight

Servo Wiring:
  ORANGE/YELLOW → Signal (PWM)
  RED           → 5V Power
  BROWN/BLACK   → Ground

Power Wiring:
  RED    → Positive (+)
  BLACK  → Negative (-)
```

## Comparison: Before vs After

```
BASIC DB METER          PAN/TILT DB METER
══════════════          ═════════════════

    ┌─────┐                 ┌─────┐
    │ dB  │                 │ dB  │
    │Gauge│                 │Gauge│
    └──┬──┘                 └──┬──┘
       │                       │
  ╔════╧════╗             ╔═══╧═══╗
  ║  XIAO   ║             ║ XIAO  ║
  ║ ESP32S3 ║             ║ESP32S3║
  ╚════╤════╝             ╚═══╤═══╝
       │                      ═╧═ Tilt
       │                     ╱   ╲
   [USB Port]              [Servo]
                               │
  Stationary              ════╧════ Pan
  Single point                ╱  ╲
  measurement              [Servo]
                               │
  Cost: ~$35            ═══════╧═══════
                        ║   Base      ║
                        ║  Platform   ║
                        ╚═════════════╝
                        
                        360° scanning
                        Room mapping
                        Auto tracking
                        
                        Cost: ~$50-70
```

## Real-World Scale Reference

```
ACTUAL SIZE COMPARISON
══════════════════════

┌────────────────┐
│                │
│  iPhone 14     │  157mm tall
│  (6.1" screen) │
│                │
│                │  ← Pan/Tilt dB Meter
│         ┌────┐ │     is about same
│         │ ● │ │     height as
│         ╞═══╡ │     a smartphone!
│         └────┘ │
│                │
└────────────────┘

Can of Soda        Pan/Tilt
┌──────┐          dB Meter
│      │          ┌────┐
│ COLA │          │ ●  │
│      │  ←→      ╞════╡
│      │          └────┘
└──────┘
122mm            145mm
```

## Summary Specifications

```
╔═══════════════════════════════════════╗
║  PAN/TILT DB METER SPECIFICATIONS    ║
╠═══════════════════════════════════════╣
║ Dimensions: 80×80×145mm (W×D×H)      ║
║ Weight: 150-200g with battery         ║
║ Pan Range: 0-180° (360° optional)    ║
║ Tilt Range: 30-150° (120° usable)   ║
║ Movement Speed: 60°/sec               ║
║ Position Accuracy: ±1°                ║
║ dB Range: 40-100 dB                   ║
║ Scan Points: 57 per full scan        ║
║ Scan Time: ~2-3 minutes full room    ║
║ Power: USB-C or 3.7V LiPo           ║
║ Runtime: 4-8 hours on battery        ║
║ Control: Serial, WiFi (optional)     ║
║ Cost: $50-70 complete                 ║
╚═══════════════════════════════════════╝
```

---

**This is your complete robotic sound scanning dB meter!** 🤖📊🔊
