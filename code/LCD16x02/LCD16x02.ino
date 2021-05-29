/*
    Display debug data on an LCD 16x02 display.

    The circuit:

    * LCD RS pin to digital pin 7
    * LCD Enable pin to digital pin 8
    * LCD D4 pin to digital pin 9
    * LCD D5 pin to digital pin 10
    * LCD D6 pin to digital pin 11
    * LCD D7 pin to digital pin 12
    * LCD R/W pin to ground
    * LCD VSS pin to ground
    * LCD VCC pin to 5V
    * 10K resistor:
    * ends to +5V and ground
    * wiper to LCD VO pin (pin 3)
 */

// include the library code:
#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins.
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


void setup() {
    // Set up the LCD's number of columns and rows.
    lcd.begin(16, 2);
    lcd.clear();

    // Label our permanent UI elements.
    lcd.setCursor(0, 0);
    lcd.print("A:");
    lcd.setCursor(7, 0);
    lcd.print("D:");
}


void loop()
{
    unsigned short address = 0xDEAD;
    unsigned short data = 0xEA;

    // Display the address.
    lcd.setCursor(2, 0);
    lcd.print(address, HEX);

    // Display the data.
    lcd.setCursor(9, 0);
    lcd.print(data, HEX);
    
    // Slow it down a little.
    delay(2);
}
