#include <avr/io.h>

#define TRANSISTOR_WITH_LED PB1

int main(void) {
    DDRB |= (1 << TRANSISTOR_WITH_LED); // we set the led as output from transistor where output signal is connected to base.


    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= (1 << WGM12);               // CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
    OCR1A = 15624;                        // 1 second
    TCNT1 = 0; // we set TCNT1 as zero which count each tick and compare it value with OCR1A and whenever it reach 15624 the OCF1A will be flag and reset 

    // --- Timer0 setup (8-bit) for ~16ms tick in CTC mode ---
    TCCR0A = 0;
    TCCR0B = 0;
    TCCR0A |= (1 << WGM01);              // CTC mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1024
    OCR0A = 155;                         // ~16ms per match
    TCNT0 = 0;

    uint8_t seconds_on = 0;   // Timer1 counter for LED ON (3s) // we set second on as zero initially 
    uint8_t count_off = 0;    // Timer0 counter for LED OFF // count_ for led off is set as zero initially 
    uint8_t led_on = 1;       // LED state is true 

    PORTB |= (1 << TRANSISTOR_WITH_LED);      // start with LED ON

    while (1) {
        // when the led is on the TCNT1 will check the value with OCR1A and every time it reach the set value then we reset TIFR1 as zero and the process continue...
        if (led_on && (TIFR1 & (1 << OCF1A))) {
            TIFR1 |= (1 << OCF1A);  // clear Timer1 flag
            seconds_on++;

            if (seconds_on > 3) { // when the TIFR1 reset theree time then the led is off 
                // Switch to LED OFF phase using Timer0
                led_on = 0;
                count_off = 0;
                TCNT0 = 0; // and TCNT set as zero
                PORTB &= ~(1 << TRANSISTOR_WITH_LED); // actually turn LED OFF
            }
        }

        // --- Timer0 manual polling for LED OFF ---
        if (!led_on && (TIFR0 & (1 << OCF0A))) {
            TIFR0 |= (1 << OCF0A);  // clear Timer0 flag
            count_off++;

            if (count_off >= 100) {  // 61 * 16ms ≈ 1s
                led_on = 1;         // Back to LED ON phase
                seconds_on = 0;     // reset Timer1 counter
                PORTB |= (1 << TRANSISTOR_WITH_LED); // turn LED ON
            }
        }
    }
}


