#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED PB5
volatile uint8_t led_flag = 0;

ISR(TIMER1_COMPA_vect) {
    led_flag = 1; // set flag every 1 second
}

void timer_init() {
    DDRB |= (1 << LED);          // LED output
    TCCR1A = 0;
    TCCR1B |= (1 << WGM12);      // CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
 

    OCR1A = 15624;               // 1 second
    TIMSK1 |= (1 << OCIE1A);     // enable Timer1 compare interrupt
    sei();                        // enable global interrupts
}

void timer_led_start(){/// do not use this part i am doing exercise 3 in the same code 
    TCNT0 = 0;
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1<<CS02 )| (1<<CS00);
    OCR0A = 156;
    TIMSK0 |= (1 << OCIE0A);



}

int main(void) {
    timer_init();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // IDLE mode keeps Timer1 running

    while (1) {
        sleep_mode();            // MCU sleeps until ISR

        if (led_flag) {
            PORTB |= (1 << LED); // LED ON
            timer_led_start();      // keep ON for 0.1s
            PORTB &= ~(1 << LED);// LED OFF
            led_flag = 0;        // reset flag
        }
    }
}

