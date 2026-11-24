#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL  // Define CPU frequency (16 MHz for ATmega328P)

// **USART Baud Rate Settings**  
#define GPS_BAUD 9600
#define HMI_BAUD 115200
#define UBRR_GPS ((F_CPU / (16UL * GPS_BAUD)) - 1)
#define UBRR_HMI ((F_CPU / (16UL * HMI_BAUD)) - 1)

// **Pin Definitions**
#define LED_PIN PB0  // LED connected to Pin 5 (PB0)

// **Function Prototypes**
void USART0_init(uint16_t ubrr);   // USART0 for GPS
void USART1_init(uint16_t ubrr);   // USART1 for DWIN HMI
void USART0_send(uint8_t data);    // Send data via USART0
void USART1_send(uint8_t data);    // Send data via USART1
uint8_t USART0_receive(void);      // Receive data via USART0
uint8_t USART1_receive(void);      // Receive data via USART1
void sendLongitudeToDWIN(int16_t longitude);
void checkDWINCommands();
void readGPSData();

void setup() {
    // **Initialize LED Pin as Output**
    DDRB |= (1 << LED_PIN);  // Set PB0 as output
    PORTB &= ~(1 << LED_PIN); // Set LED OFF initially

    // **Initialize USART Communication**
    USART0_init(UBRR_GPS);  // GPS on USART0
    USART1_init(UBRR_HMI);  // DWIN HMI on USART1

    sei();  // Enable Global Interrupts (if needed)
}

int main() {
    setup();
    while (1) {
        readGPSData();       // Process GPS Data
        checkDWINCommands(); // Handle LED Commands from HMI
        _delay_ms(500);      // Prevent CPU overload
    }
}

// 📌 **USART0 Initialization for GPS (9600 Baud)**
void USART0_init(uint16_t ubrr) {
    UBRR0H = (ubrr >> 8);  // Set Baud Rate (High Byte)
    UBRR0L = ubrr;         // Set Baud Rate (Low Byte)
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable TX & RX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit Data, No Parity, 1 Stop Bit
}

// 📌 **USART1 Initialization for DWIN HMI (115200 Baud)**
void USART1_init(uint16_t ubrr) {
    UBRR1H = (ubrr >> 8);
    UBRR1L = ubrr;
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);  // Enable TX & RX
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); // 8-bit Data, No Parity, 1 Stop Bit
}

// 📌 **Send a Byte via USART0 (GPS)**
void USART0_send(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
    UDR0 = data; // Send data
}

// 📌 **Send a Byte via USART1 (DWIN HMI)**
void USART1_send(uint8_t data) {
    while (!(UCSR1A & (1 << UDRE1))); // Wait until buffer is empty
    UDR1 = data; // Send data
}

// 📌 **Receive a Byte via USART0 (GPS)**
uint8_t USART0_receive() {
    while (!(UCSR0A & (1 << RXC0))); // Wait until data is received
    return UDR0; // Return received byte
}

// 📌 **Receive a Byte via USART1 (DWIN HMI)**
uint8_t USART1_receive() {
    while (!(UCSR1A & (1 << RXC1))); // Wait until data is received
    return UDR1; // Return received byte
}

// 📌 **Process GPS Data and Extract Longitude**
void readGPSData() {
    static int16_t longitude = 0;

    if (UCSR0A & (1 << RXC0)) {  // If GPS data is available
        uint8_t gpsData = USART0_receive();  
        // Assume a function extracts longitude (mock conversion here)
        longitude = gpsData % 1000;  // Simulate extracting last 2-3 digits
        
        sendLongitudeToDWIN(longitude);
    }
}

// 📌 **Send Extracted Longitude to DWIN HMI**
void sendLongitudeToDWIN(int16_t longitude) {
    uint8_t command[] = {
        0x5A, 0xA5, 0x07, 0x82, 0x00, 0x20, 
        (uint8_t)(longitude >> 8), (uint8_t)(longitude & 0xFF)
    };
    for (uint8_t i = 0; i < sizeof(command); i++) {
        USART1_send(command[i]);  // Send each byte to DWIN HMI
    }
}

// 📌 **Check for LED Control Commands from DWIN**
void checkDWINCommands() {
    if (UCSR1A & (1 << RXC1)) {  // If data is available from HMI
        uint8_t command[6];

        for (uint8_t i = 0; i < 6; i++) {
            command[i] = USART1_receive();  // Read 6 bytes
        }

        // Validate DWIN Protocol Header
        if (command[0] == 0x5A && command[1] == 0xA5) {
            // Check if command is for LED Control (Address 0x0483)
            if (command[2] == 0x04 && command[3] == 0x83) {
                if (command[5] == 0x01) {
                    PORTB |= (1 << LED_PIN);  // Turn LED ON
                } else {
                    PORTB &= ~(1 << LED_PIN); // Turn LED OFF
                }
            }
        }
    }
}
