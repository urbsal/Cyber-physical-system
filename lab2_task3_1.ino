#include <avr/io.h>
#include <avr/interrupt.h>

ISR (TIMER1_COMPA_vect){

  PORTB ^= (1 << PB5); //mentioning we are working in port B pin PB5 where we have use XOR function 


}


void setup() {
  DDRB |=(1<<PB5);// setting PORT B PIN 5 as LED output

  TCCR1A = 0; 
  TCCR1B = (1<<WGM12); // Refrence from datasheet page 110 and and 109 where this line suggest it s clear timer on compare match
  TCCR1B = (1 << CS12); // and this line used to define pre scaler 

  OCR1A = 15625; // The frequency of AVR is 16MHz so one tick is 62.5ns where if we prescale per tick to 1024 then each tick time will be 0.000064s and the total tick needed to be one second is 15625

  //so when the tick counter reach the value it will reset back to zero 
 TIMSK1 |= (1 << OCIE1A);
sei();
}

void loop() {
  // put your main code here, to run repeatedly:

}
