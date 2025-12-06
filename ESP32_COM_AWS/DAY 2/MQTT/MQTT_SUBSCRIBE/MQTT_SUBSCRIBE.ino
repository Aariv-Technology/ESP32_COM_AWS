#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Excitel_AARIV TECHNOLOGY ";
const char* password = "1286793808";

// MQTT Broker
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Subscriber")) {
      client.subscribe("esp32/mqtt");
      Serial.println("Subscribed to topic: esp32/mqtt");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}