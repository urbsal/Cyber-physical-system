
void setup() {
  Serial.begin(38400);   // Baud rate 38400, format = 8N1 by default
}

void loop() {
  Serial.println("Hello UART");  // Transmit string + newline
  delay(500);                    // 500 ms between transmissions
}
