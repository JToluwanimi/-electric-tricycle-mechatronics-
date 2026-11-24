# Testing & Results

## Test Protocol

| Test | Procedure | Expected Result | Actual Result |
|------|-----------|-----------------|---------------|
| Speed Sensor | Rotate wheel 10 revolutions | Display 10× wheel circumference / time | ✅ Correct |
| Voltage Display | Measure battery with multimeter; compare with display | Display within ±0.2V | ✅ Within ±0.2V |
| Odometer | Roll vehicle 100 m (measured) | Display ~0.1 km | ✅ Within ±1% |
| State IDLE→RUNNING | Push vehicle | Speed > 1 km/h → state changes | ✅ |
| State RUNNING→FAULT | Simulate low battery (adjust divider) | System enters FAULT | ✅ |
| HMI Touch | Tap screen | Arduino receives command | ✅ |

## Notes

- All tests conducted on level ground
- Battery at ~50V during testing
- No high-speed (> 30 km/h) tests performed due to safety constraints
```
