#include <PinChangeInterrupt.h>
volatile  bool current_state = LOW;
const int led = 13;
const int internal_register_switch = 8;
volatile bool myled = false;

ISR (toggleLED){
 current_state = digitalRead(13);
  digitalWrite(13, !current_state);
 }


void setup() {

  Serial.begin(9600);


  pinMode(13, OUTPUT);
  pinMode(8,INPUT_PULLUP);
   
  attachPCINT(digitalPinToPCINT(8),toggleLED, FALLING);


}

void loop() {
  // put your main code here, to run repeatedly:

}