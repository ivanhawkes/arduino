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
    Connect data sensing wires to the odd number pins 23-37.
    External clock to the interupt 2 pin (INT. 0 on the MEGA)
 */

// include the library code:
#include <LiquidCrystal.h>

const int addressLineCount = 16;
const int dataLineCount = 8;

const byte clockPin = 2;
volatile byte state = LOW;

int addressLines [addressLineCount] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
int dataLines [dataLineCount] = {23, 25, 27, 29, 31, 33, 35, 37};

// Initialize the library with the numbers of the interface pins.
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


void setup() {
    // Set up the LCD's number of columns and rows.
    lcd.begin(16, 2);

    // Init serial port.
    Serial.begin(57600);

    // Attach to an external clock pulse.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(clockPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(clockPin), onClock, RISING);
}


void onClock()
{
    Serial.println("Clock!");
    pulse();
}


void pulse()
{
    unsigned short address = 0;
    unsigned short data = 0;

    lcd.clear();

    // Add all the address lines.
    for (int i = 0; i < addressLineCount; ++i)
    {
        address |= digitalRead(addressLines[i]) << i;
    }

    // Display the address.
    lcd.setCursor(0, 0);
    lcd.print("A:");
    lcd.setCursor(2, 0);

    // Zero pad if required.
    if (address < 0x0FFF)
    {
        lcd.print("0");
    }
    if (address < 0x0FF)
    {
        lcd.print("0");
    }
    if (address < 0x0F)
    {
        lcd.print("0");
    }
    lcd.print(address, HEX);

    // Add all the data lines.
    for (int i = 0; i < dataLineCount; ++i)
    {
        data |= digitalRead(dataLines[i]) << i;
    }

    // Display the data.
    lcd.setCursor(7, 0);
    lcd.print("D:");
    lcd.setCursor(9, 0);

    // Zero pad if required.
    if (data < 0x0F)
    {
        lcd.print("0");
    }
    lcd.print(data, HEX);
    
    // The address in binary.
    lcd.setCursor(0, 1);
    for (int i = 0; i < addressLineCount; ++i)
    {
        lcd.print(digitalRead(addressLines[i]));
    }

    // Let's output to the serial console as well.
    Serial.print("B:");
    Serial.print(address, BIN);
    Serial.print(" A:");
    if (address < 0x0FFF)
    {
        Serial.print("0");
    }
    if (address < 0x0FF)
    {
        Serial.print("0");
    }
    if (address < 0x0F)
    {
        Serial.print("0");
    }
    Serial.print(address, HEX);
    Serial.print(" D:");
    if (data < 0x0F)
    {
        Serial.print("0");
    }
    Serial.print(data, HEX);
    Serial.println("");

    // Flash a LED to show the clock is working.
    state = !state;
    digitalWrite(LED_BUILTIN, state);

    // Slow it down a little.
    delay(100);
}


void loop()
{
    pulse();
}
