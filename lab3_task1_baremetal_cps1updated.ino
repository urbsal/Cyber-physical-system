#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED PB5
volatile uint8_t led_flag = 0;

ISR(TIMER1_COMPA_vect) {
    led_flag = 1; 
}

void timer_init() {
    DDRB |= (1 << LED);          // LED output in pin PB5
    TCCR1A = 0;
    TCCR1B |= (1 << WGM12);      // CTC mode for TIMER1
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
    TCCR1A |= ()

    OCR1A = 15624;               // 1 second when TICK reached 15624 it count as 1 second
    TIMSK1 |= (1 << OCIE1A);     // enable Timer1 compare interrupt
    sei();                        // enable global interrupts
}



int main(void) {
    timer_init();
    set_sleep_mode(SLEEP_MODE_IDLE); // IDLE mode keeps Timer1 running

    while (1) {
        sleep_mode();            // MCU sleeps until ISR

        if (led_flag) {
            PORTB |= (1 << LED); // LED ON
            _delay_ms(100);      // keep ON for 0.1s
            PORTB &= ~(1 << LED);// LED OFF
            led_flag = 0;        // reset flag
        }
    }
}

