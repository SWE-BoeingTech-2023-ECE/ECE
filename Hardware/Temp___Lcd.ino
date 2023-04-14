//LCD1602
//You should now see your LCD1602 display the flowing characters "SUNFOUNDER" and "hello, world"
//Email:support@sunfounder.com
//Website:www.sunfounder.com
//2015.5.7 
#include <LiquidCrystal.h>// include the library code
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
/**********************************************************/
char array1[]=" SunFounder               ";  //the string to print on the LCD
char array2[]="hello, world!             ";  //the string to print on the LCD
int tim = 250;  //the value of delay time
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);
/*********************************************************/
float temp = 0;
float tempF = 0;
void setup()
{
  sensors.begin();  // Start up the library
  Serial.begin(9600);
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows: 
}
/*********************************************************/
void loop() 
{
   sensors.requestTemperatures(); 
temp = sensors.getTempCByIndex(0);
tempF = (temp * 9.0) / 5.0 + 32.0;
// print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("C  |  ");
  
// print the temperature in Fahrenheit
  Serial.print(tempF);
  Serial.println("F");
  
    lcd.setCursor(1,0);  // set the cursor to column 15, line 0
    lcd.print(tempF);
    lcd.print((char)223);
    lcd.print("F");
    delay(2000);
    lcd.clear();  //Clears the LCD screen and positions the cursor in the upper-left corner.
}
/************************************************************/
