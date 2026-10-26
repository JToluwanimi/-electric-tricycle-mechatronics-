# Electric Tricycle — Mechatronics Integration

> **Status:** Prototyped / Integrated
> **Author:** Toluwanimi Jesusina (Team Member)
> **Institution:** Bells University of Technology, Ota, Nigeria
> **Year:** 2024

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Status](https://img.shields.io/badge/Status-Prototype-blue)]()

---

## 1. What is this?

A multidisciplinary mechatronics project involving the design, fabrication, and integration of an electric tricycle. The project encompasses mechanical chassis fabrication (welding, suspension, steering), electrical power distribution, embedded control (Arduino Mega), and a Human-Machine Interface (HMI) using a DWIN 7-inch V647 display for real-time vehicle telemetry.

**My Role:** Team member responsible for mechanical design, electronic integration, HMI development, and system state management.

---

## 2. Why was it built?

To create a functional, low-cost electric tricycle prototype that demonstrates the integration of mechanical, electrical, and electronic subsystems into a cohesive vehicle platform. The project serves as a practical application of mechatronics principles in sustainable transportation.

---

## 3. Objectives

| # | Objective | Status |
|---|-----------|--------|
| 1 | Design and fabricate a steel chassis with suspension | ✅ Completed |
| 2 | Integrate electric propulsion and power distribution | ✅ Completed |
| 3 | Develop HMI dashboard for real-time telemetry | ✅ Completed |
| 4 | Implement system state management logic | ✅ Completed |

---

## 4. System Architecture

### 4.1 High-Level Block Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                              MECHANICAL SYSTEM                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  Steel       │  │  Front       │  │  Rear        │  │  Wheel       │     │
│  │  Tubular     │  │  Suspension  │  │  Axle &      │  │  Assembly    │     │
│  │  Chassis     │  │  & Steering  │  │ Differential │  │              │     │
│  └──────────────┘  └──────────────┘  └──────────────┘  └──────────────┘     │
└─────────────────────────────────────────────────────────────────────────────┘
                                      │
                                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                              ELECTRICAL SYSTEM                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  Battery     │  │  Motor       │  │  Motor       │  │  DC-DC       │     │
│  │  Pack        │  │  Controller  │  │  (BLDC)      │  │  Converter   │     │
│  │  (48V)       │  │              │  │              │  │  (48V→12V)   │     │
│  └──────────────┘  └──────────────┘  └──────────────┘  └──────────────┘     │
└─────────────────────────────────────────────────────────────────────────────┘
                                      │
                                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                              ELECTRONIC SYSTEM                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  Arduino     │  │  DWIN 7"     │  │  Sensors     │  │  Relays      │     │
│  │  UNO         │  │  V647 HMI    │  │  (Speed,     │  │  & Switches  │     │
│  │              │  │              │  │   Voltage)   │  │              │     │
│  └──────────────┘  └──────────────┘  └──────────────┘  └──────────────┘     │
└─────────────────────────────────────────────────────────────────────────────┘
```

### 4.2 Signal Flow

| Signal | Source | Destination | Protocol |
|--------|--------|-------------|----------|
| Vehicle Speed | Hall Effect Sensor | Arduino Mega | Digital Pulse |
| Battery Voltage | Voltage Divider | Arduino Mega | Analog |
| Motor Current | Current Sensor | Arduino UNO | Analog |
| Display Data | Arduino Mega | DWIN HMI | UART (Serial) |
| User Input | DWIN HMI (Touch) | Arduino Mega | UART (Serial) |

---

## 5. Hardware Specifications

| Component | Specification | Purpose |
|-----------|--------------|---------|
| Chassis | Steel tubular frame (welded) | Structural support |
| Suspension | Front fork with shock absorbers | Ride comfort |
| Steering | Handlebar with linkage | Direction control |
| Motor | BLDC Hub Motor | Propulsion |
| Motor Controller | 48V BLDC Controller | Speed control |
| Battery | 48V Li-ion / Lead-Acid Pack | Power source |
| DC-DC Converter | 48V to 12V | Power for electronics |
| Microcontroller | Arduino UNO | System control |
| Display | DWIN 7" V647 HMI | User interface |
| Speed Sensor | Hall Effect | Speed measurement |
| Voltage Sensor | Resistor Divider | Battery monitoring |

---

## 6. Software & Firmware

### 6.1 Arduino UNO Firmware

- **Language:** C++ (Arduino IDE)
- **Libraries:** DWIN HMI Library, SoftwareSerial
- **Functions:**
  - Read speed sensor (interrupt-based)
  - Read battery voltage (analog)
  - Calculate distance (odometer)
  - Send data to DWIN display (UART)
  - Handle user input from touchscreen

### 6.2 DWIN HMI Configuration

- **Display:** DWIN 7" V647 (800x480)
- **Software:** DWIN DGUS Tool
- **UI Elements:**
  - Speedometer (dynamic arc)
  - Battery percentage (bar graph)
  - Distance (numeric display)
  - System status indicators

### 6.3 System State Logic

```
States:
- IDLE: System off, display standby
- RUNNING: Motor active, telemetry updating
- CHARGING: Battery charging, display shows charge status
- FAULT: Error detected (over-voltage, over-current, sensor failure)

Transitions:
IDLE → RUNNING: Key switch ON, throttle engaged
RUNNING → FAULT: Voltage < threshold OR current > threshold
FAULT → IDLE: Reset button pressed
```

---

## 7. Engineering Design Decisions

| Decision | Rationale | Trade-off |
|----------|-----------|-----------|
| **Steel Tubular Chassis** | High strength, weldable, low cost | Heavier than aluminium |
| **DWIN HMI** | Serial interface, easy to program, robust | Limited processing power |
| **Arduino UNO** | Sufficient I/O, low cost, familiar ecosystem | Not automotive-grade |
| **Hall Effect Speed Sensor** | Non-contact, reliable, low cost | Requires magnet mounting |
| **Voltage Divider** | Simple, no additional IC required | Accuracy depends on resistor tolerance |

---

## 8. Implementation

### 8.1 Mechanical Fabrication

1. Cut steel tubes to length
2. Welded chassis frame (MIG welding)
3. Assembled front suspension and steering
4. Mounted rear axle and wheels
5. Installed motor and controller

### 8.2 Electrical Integration

1. Wired battery pack to motor controller
2. Installed DC-DC converter for 12V rail
3. Connected sensors to Arduino
4. Wired DWIN display to Arduino (UART)

### 8.3 HMI Development

1. Designed UI screens in DWIN DGUS Tool
2. Programmed Arduino to send telemetry data
3. Configured display variables and animations
4. Tested touch input and state transitions

---

## 9. Testing & Results

| Test | Procedure | Result |
|------|-----------|--------|
| Speed Sensor | Rotate wheel manually, verify display | ✅ Speed reads correctly |
| Voltage Display | Measure battery, compare with display | ✅ Within ±0.2V |
| Distance Odometer | Roll vehicle known distance | ✅ Within ±1% |
| State Transitions | Trigger fault conditions | ✅ System enters FAULT state |

**Limitations:**
- No GPS tracking
- No battery management system (BMS) integration
- No regenerative braking

---

## 10. Limitations

1. **Battery Monitoring** — Simple voltage divider, not a full BMS.
2. **No GPS** — No location tracking or route logging.
3. **Environmental** — Display is not waterproof.
4. **Scalability** — Arduino Mega not suitable for complex control algorithms.

---

## 11. Future Development

| Priority | Enhancement | Expected Impact |
|----------|-------------|-----------------|
| 1 | Integrate BMS for cell-level monitoring | Safer battery operation |
| 2 | Add GPS module | Location tracking, route logging |
| 3 | Upgrade to STM32 or ESP32 | Faster processing, more features |
| 4 | Implement regenerative braking | Improved energy efficiency |
| 5 | Add IoT connectivity (Wi-Fi/GSM) | Remote monitoring |

---

## 12. Lessons Learned

1. **Mechanical-Electrical Integration** — Routing wires through a welded chassis requires careful planning to avoid pinching and interference.
2. **Welding Precision** — Thin-walled steel tubes require lower amperage to prevent burn-through.
3. **HMI Design** — User interface must be readable in bright sunlight; contrast and font size matter.
4. **System Grounding** — Proper grounding is critical to prevent noise in analog sensor readings.

---

## 13. Media

*[Insert photos of chassis, suspension, dashboard, and wiring]*

---

## 14. Repository Structure

```
electric-tricycle-mechatronics/
├── README.md
├── LICENSE
├── docs/
│   ├── System_Architecture.md
│   ├── Mechanical_Design.md
│   ├── HMI_Design.md
│   └── Testing.md
├── hardware/
│   ├── wiring_diagram.png
│   └── BOM.csv
├── firmware/
│   ├── arduino_mega/
│   └── dwin_config/
├── media/
│   ├── photos/
│   └── videos/
└── data/
    └── test_results.csv
```

---

## 15. License

MIT License — see [LICENSE](LICENSE) for details.

---

## 16. Contact

**Toluwanimi Jesusina**
- Email: jesusinatoluwanimi@gmail.com(mailto:jesusinatoluwanimi@gmail.com)
- LinkedIn: [linkedin.com/in/toluwanimi-jesusina](https://linkedin.com/in/your-profile)
```
