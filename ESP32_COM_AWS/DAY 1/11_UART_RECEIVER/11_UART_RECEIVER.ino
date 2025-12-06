// ---------------------------
// UART Receiver (ESP32-B)
// ---------------------------

// UART pins (must match Sender's connection)
#define TXD_PIN 17    // ESP32-B TX pin → connect to ESP32-A RX
#define RXD_PIN 16    // ESP32-B RX pin ← connect to ESP32-A TX

void setup() {
  Serial.begin(115200);  // Start USB Serial Monitor for debugging

  // Initialize UART2:
  // 9600 baud rate
  // SERIAL_8N1 = 8 data bits, No parity, 1 stop bit
  // RX pin, TX pin
  Serial2.begin(9600, SERIAL_8N1, RXD_PIN, TXD_PIN);

  Serial.println("UART Receiver Ready...");
}

void loop() {

  // Check if UART2 (ESP32-A → ESP32-B) has received data
  if (Serial2.available()) {

    // Read incoming string until newline character '\n'
    String data = Serial2.readStringUntil('\n');

    // Print received data on Serial Monitor
    Serial.print("Received: ");
    Serial.println(data);
  }
}
