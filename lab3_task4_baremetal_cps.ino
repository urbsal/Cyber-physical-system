#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
volatile bool ledstate = false;
#define BUTTON_PIN 2  // INT0 (PD2)
#define LED_PIN    PB5

volatile bool wakeFlag = false;


ISR(INT0_vect) {
    wakeFlag = true; 
}

void int0_init() {
    DDRD &= ~(1 << PD2);    // Set PD2 as input which is an external interrupt in ATmega
    PORTD |= (1 << PD2);    
    EICRA |= (1 << ISC01);  // Falling edge triggers INT0 based on data sheet
    EIMSK |= (1 << INT0);   // Enable INT0 interrupt
}

void setup() {
    DDRB |= (1 << LED_PIN); // LED as output
    int0_init();            // Initialize INT0
    sei();                  // Enable global interrupts
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set deep sleep
}


uint16_t read_ADC() {
    ADMUX = (1 << REFS0);               
    ADCSRA = (1 << ADEN)                
           | (1 << ADSC)               
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
    while (ADCSRA & (1 << ADSC));       // Wait
    return ADC;
}

void enterDeepSleep() {
    sleep_enable();
    sei();       // Make sure interrupts are enabled
    sleep_cpu(); // Go to sleep
    sleep_disable();
}

void loop() {
    
    if (!wakeFlag) {
        enterDeepSleep();
    }

    
    if (wakeFlag) {
        if (ledstate) {
            PORTB &= ~(1 << LED_PIN); // Turn off LED
            ledstate = false;
        } else {
            PORTB |= (1 << LED_PIN);  // Turn on LED
            ledstate = true;
        }

        // Read ADC if needed
        uint16_t potValue = read_ADC();

    
        wakeFlag = false;
    }
}
