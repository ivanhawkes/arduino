/*
    Display debug data on an LCD 16x02 display. It can decode and display 16 address lines as hex and binary values.

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

    Connect address sensing wires to the even number pins 22-52.
 */

// include the library code:
#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins.
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


void setup() {
    // Set up the LCD's number of columns and rows.
    lcd.begin(16, 2);
}


void loop()
{
    unsigned short address = 0; //0xDEAD;
    unsigned short data = 0; //0xEA;

    lcd.clear();

    address = digitalRead(22);
    address += digitalRead(24) << 1;
    address += digitalRead(26) << 2;
    address += digitalRead(28) << 3;
    address += digitalRead(30) << 4;
    address += digitalRead(32) << 5;
    address += digitalRead(34) << 6;
    address += digitalRead(36) << 7;
    address += digitalRead(38) << 8;
    address += digitalRead(40) << 9;
    address += digitalRead(42) << 10;
    address += digitalRead(44) << 11;
    address += digitalRead(46) << 12;
    address += digitalRead(48) << 13;
    address += digitalRead(50) << 14;
    address += digitalRead(52) << 15;

    // Display the address.
    lcd.setCursor(0, 0);
    lcd.print("A:");
    lcd.setCursor(2, 0);
    lcd.print(address, HEX);

    // Display the data.
    lcd.setCursor(7, 0);
    lcd.print("D:");
    lcd.setCursor(9, 0);
    lcd.print(data, HEX);
    
    // The address in binary.
    lcd.setCursor(0, 1);
    lcd.print(digitalRead(22));
    lcd.print(digitalRead(24));
    lcd.print(digitalRead(26));
    lcd.print(digitalRead(28));
    lcd.print(digitalRead(30));
    lcd.print(digitalRead(32));
    lcd.print(digitalRead(34));
    lcd.print(digitalRead(36));
    lcd.print(digitalRead(38));
    lcd.print(digitalRead(40));
    lcd.print(digitalRead(42));
    lcd.print(digitalRead(44));
    lcd.print(digitalRead(46));
    lcd.print(digitalRead(48));
    lcd.print(digitalRead(50));
    lcd.print(digitalRead(52));

    // Slow it down a little.
    delay(200);
}
