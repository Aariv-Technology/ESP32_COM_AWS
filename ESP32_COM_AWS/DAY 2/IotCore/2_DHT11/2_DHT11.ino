#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "aws_certificates.h"  
#include "DHT.h" 

// Wi-Fi Credentials
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

// AWS IoT Core Endpoint & MQTT Topic
const char* aws_endpoint = "YOUR ENDPOINT"; 
const char* mqtt_topic = "esp32_dht11";

// AWS IoT Client
WiFiClientSecure espClient;
PubSubClient client(espClient);

// DHT11 Configuration
#define DHTPIN 4        
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

// Function to connect to WiFi
void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

// Function to connect to AWS IoT Core
void connectAWSIoT() {
  espClient.setCACert(AWS_ROOT_CA);
  espClient.setCertificate(AWS_DEVICE_CERT);
  espClient.setPrivateKey(AWS_PRIVATE_KEY);
  client.setServer(aws_endpoint, 8883);

  while (!client.connected()) {
    Serial.print("Connecting to AWS IoT...");
    String clientId = "ESP32_" + WiFi.macAddress();
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

// Function to publish DHT11 data
void publishDHT11Data() {
  float temperature = dht.readTemperature();  
  float humidity = dht.readHumidity();        

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Create JSON payload
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  
  char payload[200];
  serializeJson(jsonDoc, payload);

  // Publish data to AWS IoT Core
  if (client.publish(mqtt_topic, payload)) {
    Serial.println("Data published: ");
    Serial.println(payload);
  } else {
    Serial.println("Failed to publish data!");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  connectWiFi();
  connectAWSIoT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!client.connected()) {
    connectAWSIoT();
  }
  client.loop();
  publishDHT11Data();
  delay(5000);
}
