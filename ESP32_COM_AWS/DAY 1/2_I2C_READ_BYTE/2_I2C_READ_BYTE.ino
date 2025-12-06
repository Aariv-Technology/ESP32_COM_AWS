#include <Wire.h>  // I2C library for communication
#define led 2      // LED connected to GPIO 2

byte receivedData;  // variable to store received byte

// This function runs automatically whenever data arrives from the master
void receiver(int n_bytes) {
  while (Wire.available()) {                       // check if any bytes are left to read
    receivedData = Wire.read();                          // read one incoming byte
    Serial.printf("Data Received: %d\n", receivedData);  // print received value for debugging
  }
}

void setup() {
  Serial.begin(115200);  // start serial monitor
  pinMode(led, OUTPUT);  // set LED pin as output

  Wire.begin(0x10);          // start I2C in SLAVE mode with address 0x10
  Wire.onReceive(receiver);  // register function to run when data is received
}

void loop() {
  // Control LED based on received data
  if (receivedData == 0) {
    digitalWrite(led, LOW);  // turn LED OFF
  } else {
    digitalWrite(led, HIGH);  // turn LED ON
  }
}
