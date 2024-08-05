#ifndef __LCD__
#define __LCD__

/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 6
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 4
 * LCD D7 pin to digital pin 3
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 */

#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

void LCD_Setup() ;
// initialize the library with the numbers of the interface pins
//Adafruit_LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

void LCD_Setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
}

#endif