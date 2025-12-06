#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "aws_certificates.h"

// Wi-Fi Credentials
const char* ssid = "Aariv_Technology";
const char* password = "1286793808";

// AWS IoT Core Endpoint & MQTT Topic
const char* aws_endpoint = "a223f6z9oxujhi-ats.iot.ap-south-1.amazonaws.com"; 
const char* mqtt_topic = "esp32_led";

// AWS IoT Client
WiFiClientSecure espClient;
PubSubClient client(espClient);

// LED Pin
const int ledPin = 2;

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void connectAWSIoT() {
  espClient.setCACert(AWS_ROOT_CA);
  espClient.setCertificate(AWS_DEVICE_CERT);
  espClient.setPrivateKey(AWS_PRIVATE_KEY);
  client.setServer(aws_endpoint, 8883);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Connecting to AWS IoT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("Connected!");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

// MQTT Callback Function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.println("Message: " + message);

  if (message == "ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED turned ON");
  } else if (message == "OFF") {
    digitalWrite(ledPin, LOW);
    Serial.println("LED turned OFF");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  connectWiFi();
  connectAWSIoT();
}

void loop() {
  if (!client.connected()) {
    connectAWSIoT();
  }
  client.loop();
}
