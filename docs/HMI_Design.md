# HMI Design — DWIN 7" V647

## Display Specifications

| Parameter | Value |
|-----------|-------|
| Model | DWIN 7" V647 |
| Resolution | 800 × 480 |
| Interface | UART (115200 baud) |
| Configuration Tool | DWIN DGUS |

## UI Screens

### Screen 1 — Dashboard (Main)

- Speedometer arc (0–60 km/h)
- Battery percentage bar
- Distance odometer (km)
- Status indicators (IDLE / RUNNING / CHARGING / FAULT)

### Screen 2 — Diagnostics

- Raw battery voltage
- Motor current (if current sensor available)
- Error log

## Communication Protocol

DWIN HMI uses a proprietary frame format:

```
5A A5 [Length] [Command] [VP Address] [Data]
```

Example: Write integer 100 to VP 0x1000:

```
5A A5 05 82 10 00 00 64
```

## Configuration File

The DWIN DGUS project file (`*.dgus`) is not included in this repository (large binary). The UI was designed with the following VP (Variable Pointer) mappings:

| VP Address | Variable | Type |
|------------|----------|------|
| 0x1000 | Speed (×10) | Integer |
| 0x1001 | Battery (%) | Integer |
| 0x1002 | Distance (×10 km) | Integer |
| 0x1003 | System State | Integer |
```

