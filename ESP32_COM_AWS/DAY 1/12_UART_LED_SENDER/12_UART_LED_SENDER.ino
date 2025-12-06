// --------------------------------------------------------
// ESP32 : UART Sender
// Sends "ON" / "OFF" to another ESP32 through UART
// --------------------------------------------------------

// Define UART pins
#define TXD_PIN 17    // ESP32-A TX pin → Goes to RX of ESP32-B
#define RXD_PIN 16    // ESP32-A RX pin ← Comes from TX of ESP32-B (optional)

// --------------------------------------------------------
void setup() {
  Serial.begin(115200);    // USB Serial Monitor for user input

  // Initialize UART2 (Serial2) with:
  // Baud rate = 9600
  // Format = 8 data bits, No parity, 1 stop bit
  // RX pin, TX pin
  Serial2.begin(9600, SERIAL_8N1, RXD_PIN, TXD_PIN);

  Serial.println("Type ON or OFF to control LED on ESP32-B");
}

// --------------------------------------------------------
void loop() {

  // Check if user typed something in Serial Monitor (USB)
  if (Serial.available()) {

    // Read the full message until user presses ENTER
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();  // Remove unwanted spaces/newlines

    // Send the command to ESP32-B via UART
    Serial2.println(cmd);

    // Print confirmation
    Serial.print("Sent: ");
    Serial.println(cmd);
  }
}
