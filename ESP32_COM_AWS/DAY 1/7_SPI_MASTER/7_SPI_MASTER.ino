#include <SPI.h>  // SPI communication library

//SPIClass spi = SPIClass(VSPI);  // Use VSPI
 SPIClass spi = SPIClass(HSPI);   // Use HSPI

#define CS 5  // Chip Select pin

//SPI pin definitions for ESP32 (custom pins)
// #define SCK 4   // Clock pin
// #define MISO 21  // Master In, Slave Out
// #define MOSI 22  // Master Out, Slave In

void setup() {
  Serial.begin(115200);  // Start Serial Monitor
  spi.begin();           // Use default pins automatically
  // spi.begin(SCK, MISO, MOSI, CS);  // Initialize SPI with custom pins

  pinMode(CS, OUTPUT);     // CS must be manually controlled
  digitalWrite(CS, HIGH);  // Keep CS HIGH when idle

  Serial.println("SPI Master Ready");
}

void loop() {

  // ------------ SEND 1 TO SLAVE (LED ON) ------------
  digitalWrite(CS, LOW);   // Activate slave
  spi.transfer(1);         // Send one byte (1)
  digitalWrite(CS, HIGH);  // Deactivate slave

  Serial.println("Sent 1");  // Debugging info
  delay(1000);

  // ------------ SEND 0 TO SLAVE (LED OFF) ------------
  digitalWrite(CS, LOW);   // Activate slave
  spi.transfer(0);         // Send one byte (0)
  digitalWrite(CS, HIGH);  // Deactivate slave

  Serial.println("Sent 0");  // Debugging info
  delay(1000);
}
