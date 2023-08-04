#include <OneWire.h>

#include <DallasTemperature.h>

#include <LiquidCrystal_I2C.h>


#include <DebugMacros.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"

#include <LiquidCrystal_I2C.h>

// Reed Switch pin
#define REED_PIN 13

// I2C address of LCD
LiquidCrystal_I2C lcd(0x27,16,2);

// GPIO where the DS18B20 is connected to
const int oneWireBus = 14;  
   

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


// Enter network credentials:
const char* ssid     = "OnePlus Nord";
const char* password = "Yuvi4213";

String str;

// Enter Google Script Deployment ID:
const char *GScriptId = "AKfycbxoR8RvYFDq2Ybefv8fGDbxIhZbPgo37aGycqoffJTH40BtwbVpsDCJCjnGBbUtSnnf";

// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

// Google Sheets setup (do not edit)
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  // initialize LCD
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on

  // Start the DS18B20 sensor
  sensors.begin();

  // Enable internal pull-up for the reed switch
  pinMode(REED_PIN, INPUT_PULLUP);  
  // initialize pin for LED
  pinMode(12, OUTPUT);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); 
  Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
    
  Serial.print("Connecting to ");
  Serial.println(host);

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i=0; i<5; i++){ 
    int retval = client->connect(host, httpsPort);
    if (retval == 1){
      flag = true;
      Serial.println("Connected");
      break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    return;
  }
  delete client;    // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object

  delay(2000);
}
void loop() { // run over and over

  static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr){
    if (!client->connected()){
      client->connect(host, httpsPort);
    }
  }
  else{
    Serial.println("Error creating client object!");
  }

  // Wait for the temperature to be ready
  sensors.requestTemperatures();
  // Read the temperature in Celsius and Fahrenheit
  float celsius = sensors.getTempCByIndex(0);
  float farenheit = (celsius * 1.8) + 32.0;

  // output data to the LCD
  lcd.setCursor(1,0);  // set the cursor to column 15, line 0
  lcd.print(farenheit);
  lcd.print((char)223);
  lcd.print("F");

  delay(100);
  // Read the state of the switch
  int proximity = digitalRead(REED_PIN); 
  int reed = 0;
  
  if (proximity == LOW) { // If the pin reads low, the switch is closed.
    digitalWrite(12, LOW); // LED off when door closed
    reed = 0;
  }
  else {
    digitalWrite(12, HIGH); // LED on when door open
    reed = 1;
  }
  delay(100);

  // Convert numbers to strings
  char value0[10];
  dtostrf(farenheit,5,2,value0);
  String value1 = String(reed);
  
  // Create json object string to send to Google Sheets
  payload = payload_base + "\"" + value0 + "," + value1 + "," + "\"}";

  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if(client->POST(url, host, payload)){ 
    // do stuff here if publish was successful
  }
  else{
    // do stuff here if publish was not successful
    Serial.println("Error while connecting");
  }

  // a delay of several seconds is required before publishing again    
  delay(5000);
}
