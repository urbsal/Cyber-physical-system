
#include <avr/interrupt.h>
#include <avr/io.h>

ISR(INT0_vect){

  PORTB ^= (1 << PB5);



}

void setup() {
  // put your setup code here, to run once:
DDRB |= (1 << PB5);
DDRD  &= ~( 1<< PD2);
PORTD |= (1 << PD2);



EICRA |= (1 << ISC01);
EICRA &= ~(1 << ISC00 );

EIMSK |= (1 << INT0);

sei();}



void loop() {
  // put your main code here, to run repeatedly:

}
