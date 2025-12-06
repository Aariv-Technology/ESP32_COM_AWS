// ---------------------------
// UART Sender (ESP32-A)
// ---------------------------

// UART pins (use any available GPIO)
#define TXD_PIN 17    // ESP32-A TX pin → connect to ESP32-B RX
#define RXD_PIN 16    // ESP32-A RX pin ← connect to ESP32-B TX

void setup() {
  Serial.begin(115200);  // Start USB Serial Monitor for debugging

  // Initialize UART2:
  // 9600 baud rate
  // SERIAL_8N1 = 8 data bits, No parity, 1 stop bit
  // RX pin, TX pin
  Serial2.begin(9600, SERIAL_8N1, RXD_PIN, TXD_PIN);

  Serial.println("UART Sender Ready...");
}

void loop() {
  // Send text to Receiver over UART2
  Serial2.println("Hello from ESP32-A");

  // Show confirmation on Serial Monitor
  Serial.println("Sent: Hello from ESP32-A");

  // Wait 1 second before sending again
  delay(1000);
}
