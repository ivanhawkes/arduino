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

#include <LiquidCrystal.h>


const int addressLineCount = 16;
const int dataLineCount = 8;
volatile unsigned short address = 0;
volatile unsigned short data = 0;

const byte clockPin = 2;
volatile byte state = LOW;

volatile unsigned long lastTime = 0;
volatile unsigned long currentTime = 0;
volatile unsigned long elapsedTime = 0;
volatile unsigned long elapsedTimeSinceOutput = 0;
volatile float clockRate = 0.0f;

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
    address = 0;
    data = 0;

    // Add all the address lines.
    for (int i = 0; i < addressLineCount; ++i)
    {
        address |= digitalRead(addressLines[i]) << i;
    }

    // Add all the data lines.
    for (int i = 0; i < dataLineCount; ++i)
    {
        data |= digitalRead(dataLines[i]) << i;
    }

    // Flash a LED to show the clock is working.
    state = !state;
    digitalWrite(LED_BUILTIN, state);

    // Output the elapsed milliseconds since the last pulse.
    currentTime = millis();
    elapsedTime = currentTime - lastTime;
    elapsedTimeSinceOutput += elapsedTime;
    
    // Avoid div by zero by assigning a clock rate for times when the clock comes back as zero ms.
    if (clockRate < 9999.0f)
        clockRate = 1000.0f / float (elapsedTime);
    else
        clockRate = 9999.0f;

    lastTime = currentTime;
}


void output()
{
    if (elapsedTimeSinceOutput > 100)
    {
        lcdOutput();
        serialOutput();
        elapsedTimeSinceOutput = 0;
    }
}


void serialOutput()
{
    // Address in binary.
    Serial.print("B:");
    Serial.print(address, BIN);
    
    // Address in HEX, padded.
    Serial.print(" A:");
    if (address <= 0x0FFF)
    {
        Serial.print("0");
    }
    if (address <= 0x0FF)
    {
        Serial.print("0");
    }
    if (address <= 0x0F)
    {
        Serial.print("0");
    }
    Serial.print(address, HEX);

    // Data in HEX, padded.
    Serial.print(" D:");
    if (data <= 0x0F)
    {
        Serial.print("0");
    }
    Serial.print(data, HEX);

    // Elapsed time in MS.
    Serial.print(" MS:");
    Serial.print(elapsedTime);

    // Clock rate in hertz.
    Serial.print(" Hertz:");
    if (clockRate < 9999.0f)
        Serial.print(clockRate);
    else
        Serial.print("----");

    // Terminate the log line.
    Serial.println("");
}


void lcdOutput()
{
    lcd.clear();

    // Display the address.
    lcd.setCursor(0, 0);
    lcd.print("A:");
    lcd.setCursor(2, 0);

    // Zero pad if required.
    if (address <= 0x0FFF)
    {
        lcd.print("0");
    }
    if (address <= 0x0FF)
    {
        lcd.print("0");
    }
    if (address <= 0x0F)
    {
        lcd.print("0");
    }
    lcd.print(address, HEX);

    // Display the data.
    lcd.setCursor(7, 0);
    lcd.print("D:");
    lcd.setCursor(9, 0);

    // Zero pad if required.
    if (data <= 0x0F)
    {
        lcd.print("0");
    }
    lcd.print(data, HEX);
    
    // The address in binary.
    lcd.setCursor(0, 1);
    for (int i = addressLineCount - 1; i >= 0 ; --i)
    {
        lcd.print(digitalRead(addressLines[i]));
    }

    // Elapsed time in MS.
    // lcd.setCursor(12, 0);
    // if (elapsedTime < 10000)
    //     lcd.print(elapsedTime);
    // else
    //     lcd.print("----");        

    // Clock rate in hertz.
    lcd.setCursor(12, 0);
    if (clockRate < 1000)
        lcd.print(clockRate);
    else if (clockRate < 9999.0f)
        lcd.print(clockRate / 1000.0f);
    else
        lcd.print("----");
}


void loop()
{
    // Update the output devices.
    output();

    // Slow it down a little.
    delay(10);
}
