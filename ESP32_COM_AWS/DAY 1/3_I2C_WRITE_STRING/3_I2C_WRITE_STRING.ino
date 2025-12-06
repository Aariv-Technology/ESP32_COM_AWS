#include <Wire.h>  // I2C communication library

void setup() {
  Serial.begin(115200);  // start Serial Monitor
  Wire.begin();          // initialize I2C as MASTER
}

void loop() {

  String message = "Hello ESP32\n";  // message to send (newline helps the slave read full string)

  Wire.beginTransmission(0x10);  // start communication with slave at address 0x10
  Wire.print(message);           // send the entire string
  Wire.endTransmission();        // finish sending data

  Serial.println("Sent: " + message);  // print for debugging

  delay(1000);  // wait 1 second before sending again
}
