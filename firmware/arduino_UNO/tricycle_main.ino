
/*
 * Electric Tricycle — Main Control Firmware
 * Author: Toluwanimi Jesusina
 * Date: 2024
 * 
 * Description:
 *   Controls the electric tricycle's telemetry system, including:
 *   - Speed measurement (Hall Effect sensor)
 *   - Battery voltage monitoring
 *   - Distance odometer
 *   - DWIN 7" HMI display communication
 * 
 * NOTE: Uses Serial1 (hardware UART) for DWIN HMI communication.
 *       No SoftwareSerial library is needed.
 * 
 * Hardware:
 *   - Arduino Mega 2560
 *   - DWIN 7" V647 HMI (UART1, 115200)
 *   - Hall Effect Speed Sensor (D2, interrupt)
 *   - Voltage Divider (A0, R1=110k, R2=10k)
 */

// ============================================================================
// PIN DEFINITIONS
// ============================================================================
#define SPEED_SENSOR_PIN    2
#define VOLTAGE_SENSOR_PIN  A0

// ============================================================================
// CONSTANTS
// ============================================================================
const float WHEEL_CIRCUMFERENCE_M = 1.85;
const float VOLTAGE_DIVIDER_RATIO = 12.0;
const float ADC_REFERENCE = 5.0;
const int ADC_RESOLUTION = 1024;

// ============================================================================
// DWIN HMI VARIABLE POINTER ADDRESSES
// ============================================================================
#define VP_SPEED        0x1000
#define VP_BATTERY      0x1001
#define VP_DISTANCE     0x1002
#define VP_STATE        0x1003

// ============================================================================
// SYSTEM VARIABLES
// ============================================================================
volatile unsigned long pulseCount = 0;
unsigned long lastPulseTime = 0;
float currentSpeed_kmh = 0.0;
float totalDistance_km = 0.0;
float batteryVoltage = 0.0;

// ============================================================================
// STATE MACHINE
// ============================================================================
enum VehicleState {
  STATE_IDLE,
  STATE_RUNNING,
  STATE_CHARGING,
  STATE_FAULT
};

VehicleState vehicleState = STATE_IDLE;

// ============================================================================
// INTERRUPT SERVICE ROUTINE — SPEED SENSOR
// ============================================================================
void onSpeedPulse() {
  unsigned long currentTime = millis();
  if (currentTime - lastPulseTime > 10) {
    pulseCount++;
    lastPulseTime = currentTime;
  }
}

// ============================================================================
// SETUP
// ============================================================================
void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);

  pinMode(SPEED_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), onSpeedPulse, FALLING);

  Serial.println("=== Electric Tricycle HMI Initialized ===");
  Serial.println("State: IDLE");

  delay(1000);
}

// ============================================================================
// MAIN LOOP
// ============================================================================
void loop() {
  static unsigned long lastSpeedUpdate = 0;
  if (millis() - lastSpeedUpdate >= 500) {
    updateSpeed();
    lastSpeedUpdate = millis();
  }

  static unsigned long lastVoltageUpdate = 0;
  if (millis() - lastVoltageUpdate >= 1000) {
    updateBattery();
    lastVoltageUpdate = millis();
  }

  static unsigned long lastHmiUpdate = 0;
  if (millis() - lastHmiUpdate >= 200) {
    sendToHMI();
    lastHmiUpdate = millis();
  }

  updateStateMachine();
}

// ============================================================================
// SPEED CALCULATION
// ============================================================================
void updateSpeed() {
  noInterrupts();
  unsigned long pulses = pulseCount;
  pulseCount = 0;
  interrupts();

  float rps = (pulses / 2.0) / 0.5;
  float speed_ms = rps * WHEEL_CIRCUMFERENCE_M;
  currentSpeed_kmh = speed_ms * 3.6;

  totalDistance_km += (speed_ms * 0.5) / 1000.0;
}

// ============================================================================
// BATTERY VOLTAGE MONITORING
// ============================================================================
void updateBattery() {
  int adcValue = analogRead(VOLTAGE_SENSOR_PIN);
  float voltageAtPin = (adcValue / (float)ADC_RESOLUTION) * ADC_REFERENCE;
  batteryVoltage = voltageAtPin * VOLTAGE_DIVIDER_RATIO;
}

// ============================================================================
// DWIN HMI COMMUNICATION
// ============================================================================
void sendToHMI() {
  sendDWINValue(VP_SPEED, (int)(currentSpeed_kmh * 10));
  int batteryPct = map((int)batteryVoltage, 400, 600, 0, 100);
  batteryPct = constrain(batteryPct, 0, 100);
  sendDWINValue(VP_BATTERY, batteryPct);
  sendDWINValue(VP_DISTANCE, (int)(totalDistance_km * 10));
  sendDWINValue(VP_STATE, (int)vehicleState);
}

void sendDWINValue(uint16_t vpAddress, int value) {
  byte frame[8];
  frame[0] = 0x5A;
  frame[1] = 0xA5;
  frame[2] = 0x05;
  frame[3] = 0x82;
  frame[4] = (vpAddress >> 8) & 0xFF;
  frame[5] = vpAddress & 0xFF;
  frame[6] = (value >> 8) & 0xFF;
  frame[7] = value & 0xFF;

  Serial1.write(frame, 8);
}

// ============================================================================
// STATE MACHINE
// ============================================================================
void updateStateMachine() {
  switch (vehicleState) {
    case STATE_IDLE:
      if (currentSpeed_kmh > 1.0) {
        vehicleState = STATE_RUNNING;
        Serial.println("State transition: IDLE -> RUNNING");
      }
      break;

    case STATE_RUNNING:
      if (batteryVoltage < 42.0) {
        vehicleState = STATE_FAULT;
        Serial.println("FAULT: Low battery voltage!");
      }
      if (currentSpeed_kmh < 0.5) {
        vehicleState = STATE_IDLE;
        Serial.println("State transition: RUNNING -> IDLE");
      }
      break;

    case STATE_CHARGING:
      if (batteryVoltage > 54.0) {
        vehicleState = STATE_IDLE;
        Serial.println("State transition: CHARGING -> IDLE (Full)");
      }
      break;

    case STATE_FAULT:
      if (batteryVoltage > 44.0) {
        vehicleState = STATE_IDLE;
        Serial.println("State transition: FAULT -> IDLE (Recovered)");
      }
      break;
  }
}
```
