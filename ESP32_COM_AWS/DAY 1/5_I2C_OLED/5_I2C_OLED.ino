#include <Wire.h>              // I2C library for OLED
#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_SSD1306.h>  // OLED library

#define SCREEN_WIDTH 128     // OLED width in pixels
#define SCREEN_HEIGHT 64     // OLED height in pixels
#define OLED_RESET -1        // Reset pin not used
#define SCREEN_ADDRESS 0x3C  // I2C address of OLED

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

  Serial.begin(115200);  // Start serial monitor for debugging

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;  // Stop if display not found
  }

  display.clearDisplay();  // Clear any previous content

  // ---------------- CENTER TEXT LOGIC -----------------
  String text = "Hello World!";  // Text to display
  int16_t x1, y1;                // Variables to store top-left of text bounds
  uint16_t w, h;                 // Width and height of text bounds

  display.setTextSize(1);               // Text size (1 = normal)
  display.setTextColor(SSD1306_WHITE);  // Text color

  // Measure how big the text will be
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // Calculate coordinates to center text
  int x = (SCREEN_WIDTH - w) / 2;
  int y = (SCREEN_HEIGHT - h) / 2;

  // Set cursor to center position
  display.setCursor(x, y);

  // Print text to the buffer
  display.println(text);

  // Send buffer to OLED to display
  display.display();
}

void loop() {
  // Nothing needed here; text is static
}
