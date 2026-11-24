# Battery Voltage Divider — Final Design

## Objective

Measure a 48V nominal battery pack (40–60V range) using an Arduino ADC (0–5V).

## Design Constraints

- V_battery_max = 60V (safety margin above 58.8V full charge)
- V_ADC_max = 5.0V (Arduino ADC reference)
- R2 chosen = 10 kΩ (standard value, low impedance for ADC)

## Voltage Divider Equation

V_out = V_battery × R2 / (R1 + R2)

Solving for R1 at V_battery = 60V, V_out = 5V:

5 = 60 × 10,000 / (R1 + 10,000)
5 × (R1 + 10,000) = 600,000
5R1 + 50,000 = 600,000
5R1 = 550,000
R1 = 110,000 Ω

## Final Design

- **R1 = 110 kΩ** (standard value)
- **R2 = 10 kΩ** (standard value)
- **Divider Ratio** = (R1 + R2) / R2 = 120,000 / 10,000 = **12.0**

## Verification Table

| V_battery (V) | V_out (V) | ADC Reading (10-bit) | Battery % (mapped) |
|:---:|:---:|:---:|:---:|
| 40.0 | 3.33 | 682 | 0% |
| 48.0 | 4.00 | 819 | 40% |
| 54.0 | 4.50 | 922 | 70% |
| 58.8 | 4.90 | 1003 | 94% |
| 60.0 | 5.00 | 1023 | 100% |

## Arduino Code

```cpp
int adcValue = analogRead(A0);
float voltageAtPin = (adcValue / 1024.0) * 5.0;
float batteryVoltage = voltageAtPin * 12.0;
int batteryPct = map((int)batteryVoltage, 400, 600, 0, 100);
batteryPct = constrain(batteryPct, 0, 100);
```

## Notes

- Use **1% tolerance resistors** for accuracy
- Add a **100 nF capacitor** between A0 and GND to reduce noise
- Total current through divider at 60V: I = 60 / 120,000 = **0.5 mA** (negligible)
```
