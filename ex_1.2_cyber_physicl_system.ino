#include <WiFiNINA.h>
void setup() {
 Serial.begin(9600);
}
void loop() {
 if (WiFi.status() == WL_NO_MODULE) {
 Serial.println("WiFi module not found!");
}
 Serial.println("Scanning available networks...");
 uint8_t numNetworks = WiFi.scanNetworks();
 for (uint8_t i= 0; i < numNetworks; i++) {
 Serial.print("Network: ");
 Serial.print(WiFi.SSID(i));
 Serial.print(" | Signal Strength: ");
 Serial.print(WiFi.RSSI(i));
 Serial.print(" dBm | Encryption: ");
 Serial.println(WiFi.encryptionType(i));
 }
delay(10000);
delay(10000);
delay(10000);
}
