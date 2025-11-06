volatile  bool current_state = LOW;
const int  led = 13;
const int external_register = 2;
volatile bool myled = false;


void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2),toggle_LED,FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:


}
void toggle_LED(){
  myled=!myled;
  digitalWrite(13,myled);
}

