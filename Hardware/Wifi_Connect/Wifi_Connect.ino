#include <ESP8266WiFi.h>

// Set WiFi credentials
#define WIFI_SSID "iPhone"
#define WIFI_PASS "thalia2002"

void setup() {
  // Setup serial port
  Serial.begin(115200);
  Serial.println();

  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.println("Connecting...");
  }

  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:

}