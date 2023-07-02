// Include required libraries for components
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Data wire for temp sensor to digital pin
#define ONE_WIRE_BUS 2

// Reed Switch pin
#define REED_PIN 3

// LCD Screen pin connections
#define RS 4
#define E 6
#define D4 10
#define D5 11
#define D6 12
#define D7 13

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

float temp = 0;
float tempF = 0;

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Start the LCD
  lcd.begin(16, 2);

  // Start the DS18B20 sensor 
  sensors.begin();

  // Enable internal pull-up for the reed switch
  pinMode(REED_PIN, INPUT_PULLUP);	
  pinMode(5, OUTPUT);
}

void loop() {
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
  delay(2000);
  lcd.clear();

  // Read the state of the switch
	int proximity = digitalRead(REED_PIN); 
	
	
	if (proximity == LOW) { // If the pin reads low, the switch is closed.
		digitalWrite(5, LOW); // LED off when door closed
	}
	else {
		digitalWrite(5, HIGH); // LED on when door open
	}

  delay(1000);
}
