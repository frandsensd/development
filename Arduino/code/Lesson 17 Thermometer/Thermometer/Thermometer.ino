//www.elegoo.com
//2016.06.13

/*
Adafruit Arduino - Lesson 12. Light and Temperature
*/

#include <LiquidCrystal.h>

int tempPin = 0;
int lightPin = 1;

//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() 
{
  lcd.begin(16, 2);
}

void loop()
{
  // Display Temperature in C
  int tempReading = analogRead(tempPin);
  float tempVolts = tempReading * 5.0 / 1024.0;
  float tempC = (tempVolts - 0.5) * 100.0;
  float tempF = tempC * 9.0 / 5.0 + 32.0;
  //         ----------------
  lcd.print("Temp         F  ");
  lcd.setCursor(6, 0);
  lcd.print(tempF);
  
  // Display Light on second row
  int lightReading = analogRead(lightPin);
  lcd.setCursor(0, 1);
  //         ----------------
  lcd.print("Light           ");  
  lcd.setCursor(6, 1);
  lcd.print(lightReading);
  delay(500);
}
