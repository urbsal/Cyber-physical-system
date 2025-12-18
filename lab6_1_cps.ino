#include <SPI.h>

// ====== SPI MASTER PIN SETUP (MKR1010) ======
// Hardware SPI pins for MKR1010:
// MOSI = 8
// MISO = 10
// SCK  = 9
// SS   = choose any free GPIO pin
const int SS_PIN = 7;

// SPI Settings:
// Clock = 1 MHz
// Bit order = MSB first
// Mode = 0 (CPOL = 0, CPHA = 0)
SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0);

void setup() {
  // Serial monitor for debugging
  Serial.begin(9600);
  while (!Serial);

  // Configure SS pin (chip select)
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);  // inactive

  // Start SPI
  SPI.begin();   // uses MCU hardware pins (8,9,10)
  Serial.println("MKR1010 SPI Master Ready");
}

void loop() {
  // Example data to send to the Uno slave
  byte dataToSend = 0x55;  // alternating bits seen clearly on oscilloscope

  // Start SPI transaction
  SPI.beginTransaction(spiSettings);

  // Pull SS low to activate slave
  digitalWrite(SS_PIN, LOW);

  // Send a byte over MOSI
  SPI.transfer(dataToSend);

  // Release slave
  digitalWrite(SS_PIN, HIGH);

  // End SPI transaction
  SPI.endTransaction();

  // Small delay before next transfer
  delay(500);
}
