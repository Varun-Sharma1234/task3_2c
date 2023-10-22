#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

// please enter your sensitive data in the Secret tab
char ssid[] = "oppo A96";
char pass[] = "varunbhardwaj";

WiFiClient client;
BH1750 lightMeter;
char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/Light/with/key/your_IFTTT_key_here";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // wait for the serial port to connect.
  }
  // Connect to WiFi
  connectWiFi();
  lightMeter.begin();
}

void loop() {
  float lux = lightMeter.readLightLevel();
  if (lux > 500) {
    sendDataToIFTTT(lux);
  }
  delay(3000);
}

void connectWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void sendDataToIFTTT(float lux) {
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to server");
    String message = "GET " + PATH_NAME + "?value1=" + String(lux) + " HTTP/1.1\r\n" +
                     "Host: " + String(HOST_NAME) + "\r\n" +
                     "Connection: close\r\n\r\n";
    client.print(message);

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }
    client.stop();
    Serial.println();
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Connection to server failed");
  }
}
