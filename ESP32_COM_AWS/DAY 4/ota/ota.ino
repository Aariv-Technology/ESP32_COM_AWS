#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h> 

// WiFi credentials
const char* ssid = "Aariv_Technology";
const char* password = "1286793808";

// AWS IoT credentials
const char* mqttServer = "a223f6z9oxujhi-ats.iot.ap-south-1.amazonaws.com";
const int mqttPort = 8883;
const char* mqttTopic = "ota/update";

// AWS IoT certificates (Replace with your actual certificates)
const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char AWS_CERT_CRT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAOnweG65dYm9r7FTsHZCfAAqtUzQMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNTAyMjYwODM4
NTNaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDd5dTHSaVphHMVDo4f
VpF5yCPQNICQp1QSfmOFCn4dCFp6Z9Frbwh5QnKW4mmszNtEW35YGzpySkkKNEye
6BqJW8fYjvwzMLOGaeYOnrUrsnenbht8fDkz+2D123AMyPhfaPDM+vya1LWhPWl9
ftJdl3axH7j+QORYY/4h+i/LOo6NqdbKWVVTvQmNh765wN8yvZc/J6+dpaR2lE1o
bNo/Z+HmabIZZsJmwlLW8pYaIpLFaEVaZsZuXjIl2+OON3dKuGfbW1ltsKxkHaiZ
NfdRXz2bGnjOOYQGkCrf6HlbC7ycWcYq2YQ7Tm0mnrtitYRQi8vf131JPpgm4dOy
ZTitAgMBAAGjYDBeMB8GA1UdIwQYMBaAFO2N9QnPwaX7IWdMxvvusaWrnpBlMB0G
A1UdDgQWBBQyrqcXWLzZ0hcdZLRNekdvx3NCZjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEApw/958CpGvfTKSKnn1r4tsgn
w0hUzkoxi9icBdHu1HFedpWF+MaHoKyE9Dw4U4Tfh+kma5G3gN+rB/YN0jykJVN8
RMUAVGXcUqJ1LO0DJfeSTRu845sNijudX1MMc63LL2n+iaUfIw8PIG7+3bOxOyU+
NswegU5x66ROHu1L9Wy78kHpMKJbkAUD7xj3vuU1jOGDWVjYLMLWidM3Mkul0KKt
esBAYvedwlmeVyXJWx9h670dA4MKofLl706Wwll3JA8fA7AdIbJtqUVgym7elcCd
bqfEvykQj5FkkQ027NuzBzVLowDbD4tPs9iVWD8nlHjS/w6xAzEUpaNtihFDdw==
-----END CERTIFICATE-----
)EOF";

const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEA3eXUx0mlaYRzFQ6OH1aRecgj0DSAkKdUEn5jhQp+HQhaemfR
a28IeUJyluJprMzbRFt+WBs6ckpJCjRMnugaiVvH2I78MzCzhmnmDp61K7J3p24b
fHw5M/tg9dtwDMj4X2jwzPr8mtS1oT1pfX7SXZd2sR+4/kDkWGP+IfovyzqOjanW
yllVU70JjYe+ucDfMr2XPyevnaWkdpRNaGzaP2fh5mmyGWbCZsJS1vKWGiKSxWhF
WmbGbl4yJdvjjjd3Srhn21tZbbCsZB2omTX3UV89mxp4zjmEBpAq3+h5Wwu8nFnG
KtmEO05tJp67YrWEUIvL39d9ST6YJuHTsmU4rQIDAQABAoIBAADj9jmigwZsYKXZ
/BoGF+lcM0CXpVhvQ3gbh7y18RGewfYxynZa6LbL3Hfcir4FyG96yoQB//cLc0WW
NiSBG/InKyMkV8DA1keYzzKJ0oJNYguK9pNGUFh0DsyNx+JVQCHp4uV/sFnStrPh
8jVtt9xmSjSlwvFKQ2BTbmamgWCYZqZNRZbohw9pHbmVKW9d4WiYgtmaVFHF8v/O
BGjK3RMhF8LFNikBdeVTohofZzDm9uIcxFNB+UUuveh3nmg5j/agPa7WuTWuGK0D
WncMuzKj7NCt5ziMNUGYjG5jLeimO83ZmgUg/nCaOjsJgAxqfo6E2IdUvzehwZKz
M6/wv2UCgYEA8TLivof0ParZg+qI+PLy/NOxyCrWhKuZeE8N6q6vzpEu0iDStZwr
VcroJ2o8p6NUXJ2Yr5RwHfFVCu68LGzwukcuiM/PRBjvFecR1hszu5+BiTqZY6QG
wBAVYyj7u/BTzhUG20utX9mOZFWWoqrUBgj7tRLsQL7liqGymjhUiksCgYEA64O8
fkoRaXvxBHhGySfV49q+eki0h1EtfS9LxjIsJAWqFMWd3hpQ7z/W1Qf13pe/0dJ+
XkQ2YzncSg26mw3Wna6nvtiJHbLgY8QTJNZlRwOCvIyUpcKaIpJpO5wMksE2B8FU
/aDcvpHFEFiYEvWmJLccNcbPcq5jj4J7vrEf7ecCgYBiteWP/knZtwMf9qsp3xvM
lsKKXeW/FA9R6mCSq/0tN8OGoYI6avhdOxY5k0PRg4lyAdlL+h7MUkjwmXWPZOI8
CgekXA/CGcY3G4L0MdvBLG0yATZnTeipT6vBEjO73YYesAJHWzhQItQggdqNJgZx
ldmVaHIhqT59tuFGNkl68wKBgD2dt6Dqy+dnD8V1mj9idqkoP5FKmECW0mngB9kQ
o0/BU6Afm4/SHr6ZWyFc9cyIsxy4D2umOPx41NNjCCEs26thWG2rxSPJ+r7fO4r1
tIWBu6CMvw8m/KBy4alAyGFUv0gEIrJAdcA97uq54v+C2Tbd2TmQjBGa4l1Xa/yV
5p6dAoGBAM+1vmqIIck8gNnh+gae+yAHnPiILjkdKBfupyvrIcyySmm3Ew6AHb8v
Sm8/39TyIEjJ3M+ruYTrng8e95LgFeOtX9mbnVTC+oX2t1Ktpja5SFbnirmLBGna
IIVuvTg+PoWw7PfnodxVyC/DRSxJLBpvG0wIckrqllczZR5zv0TB
-----END RSA PRIVATE KEY-----
)EOF";

// MQTT and WiFi clients
WiFiClientSecure net;
PubSubClient client(net);

// LED pin
#define LED_PIN 2  // Built-in LED on ESP32

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received: ");
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);
    
    // Parse JSON using ArduinoJSON
    StaticJsonDocument<512> doc;  // Create a JSON document
    DeserializationError error = deserializeJson(doc, message);
    
    if (error) {
        Serial.print("JSON Parsing Failed: ");
        Serial.println(error.f_str());
        return;
    }

    // Extract firmware URL
    if (doc.containsKey("firmware_url")) {
        String firmwareURL = doc["firmware_url"].as<String>();
        Serial.println("Firmware URL: " + firmwareURL);
        performOTA(firmwareURL.c_str());
    } else {
        Serial.println("Error: Missing 'firmware_url' in JSON.");
    }
}

void performOTA(const char* firmwareURL) {
    HTTPClient http;
    http.begin(firmwareURL);
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        int contentLength = http.getSize();
        WiFiClient* stream = http.getStreamPtr();
        
        if (!Update.begin(contentLength)) {
            Serial.println("Not enough space for OTA.");
            return;
        }

        size_t written = Update.writeStream(*stream);
        if (written == contentLength && Update.end()) {
            Serial.println("OTA Success! Restarting...");
            ESP.restart();
        } else {
            Serial.println("OTA Failed!");
        }
    }
    http.end();
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
    }
    Serial.println("Connected to WiFi");

    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);
    
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    
    while (!client.connected()) {
        Serial.println("Connecting to AWS IoT...");
        if (client.connect("ESP32Client")) {
            Serial.println("Connected!");
            client.subscribe(mqttTopic);
        } else {
            Serial.print("Failed with state ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

void loop() {
    client.loop();
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
}
