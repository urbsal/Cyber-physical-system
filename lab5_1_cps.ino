#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void UART_init() {
    // Set baud rate (UBRR = 8 for 115200 baud)
    uint16_t ubrr = 8;
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    // UCSR0C settings:
    // UPM01 = 1, UPM00 = 0 → Even parity
    // USBS0 = 1 → 2 stop bits
    // UCSZ01 = 1, UCSZ00 = 0 → 7 data bits
    UCSR0C = (1 << UPM01)      // Even parity
           | (1 << USBS0)      // 2 stop bits
           | (1 << UCSZ01);    // 7-bit data

    // UCSR0B:
    // Enable transmitter, ensure UCSZ02 = 0 for 7-bit mode
    UCSR0B = (1 << TXEN0);     // Enable TX only
}

void UART_send(char data) {
    // Wait for buffer to be empty
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;               // Load data into UART data register
}

int main(void) {
    UART_init();

    while (1) {
        UART_send('A');        // send 'A' repeatedly
        _delay_ms(500);        // visible on PuTTY
    }
}