#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT Broker
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// DHT sensor
#define DHTPIN 4  // GPIO4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Publisher")) {
      Serial.println("Connected to MQTT broker");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Convert to string and publish
  char msg[50];
  snprintf(msg, 50, "Temp: %.2f C, Hum: %.2f %%", temp, hum);

  client.publish("esp32/test", msg);
  Serial.print("Published: ");
  Serial.println(msg);

  delay(5000);  // Publish every 5 seconds
}