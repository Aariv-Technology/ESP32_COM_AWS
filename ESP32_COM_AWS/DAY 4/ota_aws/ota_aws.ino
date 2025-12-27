#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "Excitel_AARIV TECHNOLOGY ";
const char* password = "1286793808";

// AWS IoT credentials
const char* mqttServer = "a223f6z9oxujhi-ats.iot.eu-north-1.amazonaws.com";
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
MIIDWTCCAkGgAwIBAgIUa0dy5EtEca6EsGFJ8xHMpAsIzJwwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MTIyNjA5NTAy
M1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANogTyQSaTYjNwUGFA/e
HSdqh678PJzgGccQ9pYR9aF0WBuoEaZH63WpjpB80iOOhPDvZH/KUyzDIGQQnQ7E
lFg9YnyHvdvsOb/gk4N5moY92i0ACb2Qh4/5LUp6wgNkZGr1Jrx6Mt0Q6l+ZbVZ1
SF0fTNSbtZFhAv8nFFwUf/+AJHEK00VLo90qKxsfsB3hsVWJtV4ou89BOLO4RNgY
1Eioq5Sf8vXmrOP7Z1rgIRS1q5gdNWTlJT6BMfq8/8mHxOP4MQAzkXjxeGB//QnX
m0hSn2iEpJ6ol3YLgfjTnCkpSwJBqKiGhePmXNkHvr2LXmI5Hz8YtYIC83DA/ZNS
Ta0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUn5wPI2BLUK2nIuQyzDyUWRXT4jUwHQYD
VR0OBBYEFLfx0yIN8qZIIdyCo221RSbT4FLlMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBU9+GMbZQLOCefuJK9ITMVwNGx
fSWC93T+D5gaard6VXcVrHP4G064FcMjl7JdFAa8b1abC93kkaQrRixLgctmX78i
6cYNQx5dqmqlQMqH9wq244WLqUQwe++SjlqVyRYy2bdK5DhhHvulZaQupmwrPWld
QwHWsuUfXA7JRa4P51GdriWr2goAFUivfClAVqAEkCzkUsjzEGn9GHpqlF0P/uVx
+MSpRIe8YKF6+VxAru7S3WuhC8EvNs9hESylhw71xixcpIkSapS2tt1XVv844Ke+
zrP3FX9cDg7NfNvBbrvuGxa9G+53p2DJPid1xhUcvFo9xzGOTKK3Q+7yFjMh
-----END CERTIFICATE-----
)EOF";

const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA2iBPJBJpNiM3BQYUD94dJ2qHrvw8nOAZxxD2lhH1oXRYG6gR
pkfrdamOkHzSI46E8O9kf8pTLMMgZBCdDsSUWD1ifIe92+w5v+CTg3mahj3aLQAJ
vZCHj/ktSnrCA2RkavUmvHoy3RDqX5ltVnVIXR9M1Ju1kWEC/ycUXBR//4AkcQrT
RUuj3SorGx+wHeGxVYm1Xii7z0E4s7hE2BjUSKirlJ/y9eas4/tnWuAhFLWrmB01
ZOUlPoEx+rz/yYfE4/gxADORePF4YH/9CdebSFKfaISknqiXdguB+NOcKSlLAkGo
qIaF4+Zc2Qe+vYteYjkfPxi1ggLzcMD9k1JNrQIDAQABAoIBAEHOlZHQbCWFIkhF
uS/AVxNZNG46cx7xDVXdbEeCS5TWQO63GfwMnqD08qnsvokzqi/2SV+0bruGcvM9
f+TAsfCdNGVVnpYHArU8z8ROn6v4ZSZGaKnkPMHW7y8VMgtL8m/CXllmlInVXKBS
7YW0BIzzsH5EK9ccaYcJBz9wf91ZAQ+fTuSWgCWl5PxuKi83PuVY0aO6T/Pta81d
WvEOWk4lT+UC1Ww6Fjcat0H93w9ViEHzP+xI5vL+qOn787CoSnJwiaUIJ268tCfO
978JyGrQGu5/+OJ6J1Hj+UdTNtbfSZFGC4swTv53V49Y0ryJdvKwjdv7vK4nPsKF
BOOPO4ECgYEA7NeR5sRn904OZ7ZRqfEhThbZWKc8RclhhkP8tRX4LRllUrrv6Jwm
SdfyegUjeXwK94Zf1aRx21oOz5QuB2EH8dMtslZtpuBg6MCB1cq7gUeUF/Od/jXI
u36AsXa84YHgk/Yg1eiUSnGY4kCiW/8p9FyMu7CpGPl3y9BgCl9ldkkCgYEA68Ut
vk7KPEaGDguh9+ohamJgTqwZoFvGvM9tJZvJL/Tmdd/rQdSNjIQFbTI+8htuD+aA
SfQ65hf1GsQh7grPiqCTBrI+UrWDhjFGt6n8Qez0HA+3ehje51w7YPXwA37cKBk8
d/88a2uXaMve6SyJP2WLPGVYc9p6GpqvQ8f+DEUCgYEArHM+SiBCb0DHnl6TUcFA
deKPOUzzxwCd4SyJybUIcpYmP7nby6w++M+jP5OUQ/W86oROLXpCaOxHEoAbj8Xo
Cf2xLURfLP39shvRrcCjDLv4qgP5y0qQMW0n+Y+yciSmSDYTrI1fxGC4aC2VypSd
kM7+rf8w8P1ca+v6sjLJ5vkCgYEAqeDQHeB9fUK/Up11HcF4REQP6n+8kzutjdvT
ZBbEtqY569LkmvFA8G8rzcs3aZD0E8bcxuyQB7xZYejSHcdyVQtwBJInr8aTtvwI
XUvAs+u80/9LD1GeQovFSQ7L2mjszoOoeznP2gKS/PJwcggh+Cj9Uw4e1pMkCclM
4YTXexUCgYEA6OwAGDQ49CbzXy3s2J2gmPhF4dmDu/lj/VoKRBx6XpvrpzQJEzRC
vmplFpi5AGjGa1LSPG5ygB/ZgvK6iX6lhiX4DscyE+z178P+Zjy/Gx85OgS9a2tn
TPY/lM909LTwJ3yKT5JBrlrJTTSmnnJpqGAtBIRArAqvufIaqDoFWbA=
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
    delay(100);
  }
  Serial.println("Connected to WiFi");

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to AWS IoT...");
    String clientId = "ESP32_" + WiFi.macAddress();

    if (client.connect(clientId.c_str())) {
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
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
