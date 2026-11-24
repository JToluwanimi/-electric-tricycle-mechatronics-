# System Architecture

## Three-Layer Design

The electric tricycle is a three-layer mechatronic system:

1. **Mechanical layer** — Chassis, suspension, steering, wheels
2. **Electrical layer** — Battery, motor controller, motor, DC-DC converter
3. **Electronic layer** — Arduino UNO, DWIN HMI, sensors, switches

## Data Flow

1. Rider turns key → 12V rail energizes → Arduino boots
2. Wheel rotates → Hall Effect sensor pulses → Arduino counts
3. Battery voltage divider → ADC reading → Arduino calculates voltage
4. Arduino formats data → sends over UART → DWIN display updates
5. Rider touches display → DWIN sends command → Arduino changes state

## Power Architecture

```
48V Battery ──┬──▶ Motor Controller ──▶ BLDC Hub Motor
              │
              └──▶ DC-DC Converter ──┬──▶ Arduino UNO (5V via USB or regulator)
                                     └──▶ DWIN HMI (12V)
```

## Sensor Interfaces

| Sensor | Interface | Pin | Notes |
|--------|-----------|-----|-------|
| Hall Effect (speed) | Interrupt (D2) | 2 | Debounced in software |
| Voltage divider | ADC | A0 | R1=110k, R2=10k |
```
