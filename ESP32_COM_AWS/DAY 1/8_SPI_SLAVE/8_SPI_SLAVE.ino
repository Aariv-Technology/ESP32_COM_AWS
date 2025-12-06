#include <ESP32SPISlave.h>  // Library for ESP32 SPI slave mode

ESP32SPISlave slave;

// We only use 1 byte for sending/receiving
uint8_t tx_buf[1] = { 0 };  // Data to send back to master
uint8_t rx_buf[1] = { 0 };  // Data received from master

void setup() {
  Serial.begin(115200);
  delay(2000);  // Small delay for Serial stability

  pinMode(2, OUTPUT);  // LED pin (or any output device)

  slave.setDataMode(SPI_MODE0);  // SPI Mode 0 (must match master)
  slave.setQueueSize(1);         // Only 1 transaction at a time

  // Start slave with default HSPI pins:
  // SCK = 14, MISO = 12, MOSI = 13, CS = 15
  slave.begin();

  Serial.println("SPI Slave Ready");
}

void loop() {
  // Prepare TX buffer before each transfer
  // It will send back whatever was received last time
  tx_buf[0] = rx_buf[0];

  // Waits here until master sends data
  size_t received = slave.transfer(tx_buf, rx_buf, 1);

  // If master sent something
  if (received > 0) {
    uint8_t message = rx_buf[0];

    Serial.print("Received: ");
    Serial.println(message);

    // Control LED based on received value
    if (message == 0)
      digitalWrite(2, LOW);
    else
      digitalWrite(2, HIGH);

    // Prepare this same value to send back on next transfer
    tx_buf[0] = message;
  }
}
