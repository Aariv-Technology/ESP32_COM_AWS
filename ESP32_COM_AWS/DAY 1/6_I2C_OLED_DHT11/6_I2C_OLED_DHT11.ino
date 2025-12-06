#include <Wire.h>              // I2C communication library
#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_SSD1306.h>  // OLED library
#include <Adafruit_Sensor.h>   // Sensor base library
#include <DHT.h>               // DHT sensor library

// ---------------- OLED SETTINGS -----------------
#define SCREEN_WIDTH 128     // OLED width in pixels
#define SCREEN_HEIGHT 64     // OLED height in pixels
#define OLED_RESET -1        // Reset pin not used
#define SCREEN_ADDRESS 0x3C  // I2C address of OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- DHT SETTINGS -----------------
#define DHTPIN 4           // Pin where DHT is connected
#define DHTTYPE DHT11      // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);  // Create DHT object

void setup() {
  Serial.begin(115200);  // Start serial monitor for debugging

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;  // Stop execution if OLED not found
  }

  dht.begin();             // Start DHT sensor
  display.clearDisplay();  // Clear OLED buffer
  display.display();       // Refresh display
}

void loop() {
  float temperature = dht.readTemperature();  // Read temperature in Celsius
  float humidity = dht.readHumidity();        // Read humidity

  // ---------------- ERROR HANDLING -----------------
  if (isnan(temperature) || isnan(humidity)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Center text: "Sensor Error!"
    String msg = "Sensor Error!";
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);
    int x = (SCREEN_WIDTH - w) / 2;
    int y = (SCREEN_HEIGHT - h) / 2;

    display.setCursor(x, y);
    display.println(msg);
    display.display();

    Serial.println("Failed to read from DHT sensor!");
    delay(2000);  // Wait before retry
    return;       // Skip rest of loop
  }

  // ---------------- NORMAL DISPLAY -----------------
  display.clearDisplay();  // Clear previous values
  display.setTextColor(SSD1306_WHITE);

  // Create text strings
  String tempText = "Temperature: " + String(temperature) + " C";
  String humText = "Humidity: " + String(humidity) + " %";

  int16_t x1, y1;
  uint16_t w, h;

  // ---------------- TEMPERATURE CENTERED -----------------
  display.getTextBounds(tempText, 0, 0, &x1, &y1, &w, &h);
  int xT = (SCREEN_WIDTH - w) / 2;
  int yT = ((SCREEN_HEIGHT - h) / 2) - 10;  // slightly above center
  display.setCursor(xT, yT);
  display.print(tempText);

  // ---------------- HUMIDITY CENTERED -----------------
  display.getTextBounds(humText, 0, 0, &x1, &y1, &w, &h);
  int xH = (SCREEN_WIDTH - w) / 2;
  int yH = ((SCREEN_HEIGHT - h) / 2) + 10;  // slightly below center
  display.setCursor(xH, yH);
  display.print(humText);

  display.display();  // Refresh OLED to show new readings

  Serial.printf("Temperature: %.2f C  Humidity: %.2f %%\n", temperature, humidity);  // Print data to Serial Monitor

  delay(2000);  // Wait 2 seconds before next reading
}
