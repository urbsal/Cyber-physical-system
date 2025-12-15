#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint16_t echo_time = 0;
volatile uint8_t echo_done = 0;

// Interrupt on INT1 for echo pulse measurement
ISR(INT1_vect)
{
    static uint8_t rising = 1;

    if (rising)
    {
        // Rising edge → start timer
        TCNT1 = 0;
        rising = 0;
        EICRA &= ~(1 << ISC10);    
        EICRA |=  (1 << ISC11);
    }
    else
    {
        // Falling edge → read timer
        echo_time = TCNT1;
        echo_done = 1;
        rising = 1;
        EICRA |=  (1 << ISC10) | (1 << ISC11);  
    }
}

void timer1_init()
{
    // Normal mode
    TCCR1A = 0x00;
    TCCR1B = (1 << CS10);
}

void pwm_init()
{
   
    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << CS01); 
    DDRD |= (1 << PD6);   
}

void ext_int_init()
{
    // INT1 on PD3
    EICRA |= (1 << ISC11) | (1 << ISC10); // Rising edge
    EIMSK |= (1 << INT1);                 // Enable INT1
}

void trigger_ultrasonic()
{
    // PD2 = trigger pin
    PORTD &= ~(1 << PD2);
    _delay_us(2);
    PORTD |=  (1 << PD2);
    _delay_us(10);
    PORTD &= ~(1 << PD2);
}

uint16_t measure_distance_cm()
{
    echo_done = 0;
    trigger_ultrasonic();

    // Wait for measurement
    while (!echo_done);

   
    return echo_time / 932;
}

void set_fan_speed(uint16_t distance)
{
    if (distance > 20) {
        OCR0A = 0;                 
    }
    else if (distance <= 4) {
        OCR0A = 255;                
    else {
        // Map 4–20 cm → PWM range 255–0
        uint8_t pwm = (uint8_t)(255 - ((distance - 4) * 255 / 16));
        OCR0A = pwm;
    }
}

int main()
{
    DDRD |= (1 << PD2);   // trigger pin output
    DDRD &= ~(1 << PD3);  // echo pin input

    timer1_init();
    pwm_init();
    ext_int_init();
    sei();               // enable global interrupts

    while (1)
    {
        uint16_t distance = measure_distance_cm();
        set_fan_speed(distance);
        _delay_ms(2000);
    }
}