
#include <avr/interrupt.h>
#include <avr/io.h>

ISR(PCINT0_vect){

  PORTB ^= (1 << PB5);// select the pin from the port which get toggle when the interruption occurs 


}

void setup() {
  // put your setup code here, to run once:
DDRB |= (1 << PB5);// MAKE the pin as output
PORTB |= (1 << PB0);// MAKE THE PIN AS INPUT
DDRD &=~(1 << PB0); // SELECT the port 



PCICR |= (1 << PCIE0);   // In this section we select the port B as our enable interrupts using PCIE0
PCMSK0 |= (1 << PCINT0);}// This part we select the pin 8 as our pin which interrupt the process


void loop() {
  // put your main code here, to run repeatedly:

}
