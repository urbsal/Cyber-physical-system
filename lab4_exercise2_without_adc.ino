#include <avr/io.h>
#include <avr/interrupt.h>

#define FAN_PWM PB1     // OC1A pin
#define PWM_STEP 50     // add 50 every second
#define PWM_MIN 50      // start at 50
#define PWM_MAX 250     // stop at 250

volatile uint16_t tick_counter = 0;// set zero
volatile uint8_t second_counter = 0;
volatile uint8_t fan_counter = 0;
volatile uint8_t fan_on = 0;
volatile uint8_t pwm_value = 0;

// ---------------- PWM ----------------
void setup_pwm() {
    DDRB |= (1 << FAN_PWM);                // pin 9 as output  OC1A output
    TCCR1A = (1 << COM1A1) | (1 << WGM10); // COM1A1 =1 which clear OC1A on compare match, set at bottom Fast PWM 8-bit 
    // WGM = 1 (PWM, phase correct, uodate ocr1a at buttom)
    TCCR1B = (1 << WGM12) | (1 << CS11);   // prescaler = 8 the lower the pre-scaler the clear the motor rotation and less noisy
    OCR1A = 0;                             // fan off initially we set OCR1A as set and with timer the OCR1A will increase and the duty cycle also increase respect to time 
}


void setup_timer0() { // this is the part we used to set the paramter for timer 0
    TCCR0A = (1 << WGM01);                // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00);   // prescaler 1024
    OCR0A = 255;                           // ~16ms // max 8 bit    
    TIMSK0 |= (1 << OCIE0A);               // enable interrupt
}
// intrupt occur when the timer0 reach 5 second and 10 second 
ISR(TIMER0_COMPA_vect) {
    tick_counter++;

    if (tick_counter >= 62) { // ~1 second
        tick_counter = 0;

        if (!fan_on) {
            second_counter++;

            if (second_counter >= 10) {   // 10 seconds OFF
                fan_on = 1;
                fan_counter = 0;
                pwm_value = PWM_MIN;      // start at 50
                OCR1A = pwm_value;
            }
        }
        else {
            fan_counter++;

            if (fan_counter <= 5) {       // 5 seconds ON

                pwm_value += PWM_STEP;    // add +50 per second
                if (pwm_value > PWM_MAX) pwm_value = PWM_MAX;

                OCR1A = pwm_value;
            }
            else {
                // turn fan OFF
                fan_on = 0;
                second_counter = 0;
                OCR1A = 0;
            }
        }
    }
}


int main(void) {
    setup_pwm();
    setup_timer0();
    sei();

    while (1) {
     
    }
}
