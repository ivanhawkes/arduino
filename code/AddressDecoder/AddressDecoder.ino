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
    65C02 Bus Enable pin to 39
    65C02 Read / Write pin to 41
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
volatile char clockStr[16];
volatile char clockMagnitude;

const int busEnablePin = 39;
int busEnableValue = LOW;
const int readWritePin = 41;
int readWriteValue = LOW;


int addressLines [addressLineCount] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
int dataLines [dataLineCount] = {23, 25, 27, 29, 31, 33, 35, 37};

// Initialize the library with the numbers of the interface pins.
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


void setup() {
    // Set up the LCD's number of columns and rows.
    lcd.begin(16, 2);

    // Init serial port.
    Serial.begin(57600);

    // Init all the address lines.
    for (int i = 0; i < addressLineCount; ++i)
    {
        pinMode(addressLines [i], INPUT);
    }

    // Init all the data lines.
    for (int i = 0; i < dataLineCount; ++i)
    {
        pinMode(dataLines [i], INPUT);
    }

    // BusEnable and ReadWrite.
    pinMode(busEnablePin, INPUT);
    pinMode(readWritePin, INPUT);

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

    // Get some flag values.
    busEnableValue = digitalRead(busEnablePin);
    readWriteValue = digitalRead(readWritePin);

    // Flash a LED to show the clock is working.
    state = !state;
    digitalWrite(LED_BUILTIN, state);

    // Output the elapsed milliseconds since the last pulse.
    currentTime = micros();
    elapsedTime = currentTime - lastTime;
    elapsedTimeSinceOutput += elapsedTime;
    
    // Avoid div by zero by assigning a clock rate for times when the clock comes back as zero ms.
    if (elapsedTime > 0.0f)
        clockRate = 1000000.0f / float (elapsedTime);
    else
        clockRate = 0.0f;

    // Turn the clock rate into a string that can be used by sprintf.
    if (clockRate >= 1000000.0f)
    {
        clockMagnitude = 'm';
        dtostrf(clockRate / 1000000.0f, 1, 1, clockStr);
    }
    else if (clockRate >= 1000.0f)
    {
        clockMagnitude = 'k';
        if (clockRate >= 100000.0f)
            dtostrf(clockRate / 1000.0f, 1, 0, clockStr);
        else if (clockRate >= 10000.0f)
            dtostrf(clockRate / 1000.0f, 1, 1, clockStr);
        else
            dtostrf(clockRate / 1000.0f, 1, 2, clockStr);

    }
    else if (clockRate >= 1.0f)
    {
        clockMagnitude = 'h';
        if (clockRate >= 100.0f)
            dtostrf(clockRate, 1, 0, clockStr);
        else if (clockRate >= 10.0f)
            dtostrf(clockRate, 1, 1, clockStr);
        else
            dtostrf(clockRate, 1, 2, clockStr);
    }

    lastTime = currentTime;
}


void output()
{
    if (elapsedTimeSinceOutput > 100000)
    {
        lcdOutput();
        serialOutput();
        elapsedTimeSinceOutput = 0;
    }
}


void serialOutput()
{
    char line[80];

    // Format the output string.
    sprintf(line, "%04X %02X %c%c %s%c", address, data,
        readWriteValue ? 'R' : 'W', 
        busEnableValue ? 'E' : 'D',
        clockStr, clockMagnitude);
    Serial.print(line);

    // Address in binary.
    Serial.print(" ");
    Serial.print(address, BIN);

    // Terminate the log line.
    Serial.println("");
}


void lcdOutput()
{
    char line[16];

    lcd.clear();

    // Format the output string.
    sprintf(line, "%04X %02X %c%c %s%c", address, data,
        readWriteValue ? 'R' : 'W', 
        busEnableValue ? 'E' : 'D',
        clockStr, clockMagnitude);

    lcd.setCursor(0, 0);
    lcd.print(line);

    // The address in binary.
    lcd.setCursor(0, 1);
    for (int i = addressLineCount - 1; i >= 0 ; --i)
    {
        lcd.print(digitalRead(addressLines[i]));
    }
}


void loop()
{
    // Update the output devices.
    output();

    // Slow it down a little.
    delay(10);
}
