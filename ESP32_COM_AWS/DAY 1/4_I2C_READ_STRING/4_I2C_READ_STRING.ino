#include <Wire.h>  // I2C communication library

String myData = "";  // variable to store the incoming string

// This function automatically runs whenever the master sends data
void receiveEvent(int numBytes) {
  myData = Wire.readStringUntil('\n');  // read characters until newline ('\n')
  Serial.print("Received: ");           // print message label
  Serial.println(myData);               // show the full received string
}

void setup() {
  Serial.begin(115200);          // start Serial Monitor
  Wire.begin(0x10);              // initialize I2C in SLAVE mode at address 0x10
  Wire.onReceive(receiveEvent);  // register receive callback function
}

void loop() {
  // Nothing required here — data is handled inside receiveEvent()
}
