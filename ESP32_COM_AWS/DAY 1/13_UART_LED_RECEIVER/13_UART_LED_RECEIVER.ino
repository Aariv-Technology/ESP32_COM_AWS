// ----------------------------------------------------------
// ESP32 : UART Receiver + LED Control
// Receives "ON" / "OFF" commands via UART2 and controls LED
// ----------------------------------------------------------

// UART pin definitions
#define TXD_PIN 17     // TX pin (not used for receiving, but required for Serial2.begin)
#define RXD_PIN 16     // RX pin (receives data from ESP32-A)

// LED pin
#define LED_PIN 2      // Onboard LED (GPIO 2)

// ----------------------------------------------------------
void setup() {

  Serial.begin(115200);  // USB Serial Monitor (debug)

  // Initialize UART2 (Serial2)
  // Parameters:
  // Baud rate = 9600
  // Data format = 8N1
  // RXD_PIN = input, TXD_PIN = output
  Serial2.begin(9600, SERIAL_8N1, RXD_PIN, TXD_PIN);

  // LED setup
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Start OFF

  Serial.println("Ready to receive commands...");
}

// ----------------------------------------------------------
void loop() {

  // Check if data arrived from ESP32-A
  if (Serial2.available()) {

    // Read full string until newline ('\n')
    String cmd = Serial2.readStringUntil('\n');
    cmd.trim();  // Remove spaces or extra characters

    // Print received message
    Serial.print("Received: ");
    Serial.println(cmd);

    // ------------ PROCESS COMMANDS -------------
    if (cmd == "ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED Turned ON");
    }
    else if (cmd == "OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED Turned OFF");
    }
  }
}
