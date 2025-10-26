#include "sam.h"
#define LED_PIN 20 // D6 → PA20 on MKR1010
#define INPUT_PIN 10 // D2 → PA10
void setup() {
 // Enable the APB clock for the PORT module
 PM->APBBMASK.reg |= PM_APBBMASK_PORT;
 // Configure LED_PIN as OUTPUT (set bit in DIRSET)
 PORT->Group[0].DIRSET.reg = (1 << LED_PIN);
 // Configure INPUT_PIN as INPUT (clear bit in DIRCLR)
 PORT->Group[0].DIRCLR.reg = (1 << INPUT_PIN);
}
void loop() {
 // Turn LED ON
 PORT->Group[0].OUTSET.reg = (1 << LED_PIN);
 delay(500);
 // Turn LED OFF
 PORT->Group[0].OUTCLR.reg = (1 << LED_PIN);
 delay(500);
}
