#include <Wire.h>  // I2C communication library

byte myData = 0;  // value to be sent to the slave

void setup() {
  Wire.begin();          // initialize I2C in master mode
  Serial.begin(115200);  // start serial monitor for debugging
}

void loop() {

  myData = !myData;  // toggle value: 0 → 1 → 0 → 1 ...

  Wire.beginTransmission(0x10);  // start communication with slave (address 0x10)
  Wire.write(myData);            // send the single byte
  Wire.endTransmission();        // send data and release the I2C bus

  Serial.print("Sent: ");
  Serial.println(myData);  // display the value sent to the slave

  delay(1000);  // wait 1 second before sending again
}
