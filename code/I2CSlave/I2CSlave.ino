#include <Wire.h>

const uint8_t I2C_ADDRESS = 0x42;
const byte clockPin = 2;
const byte dataPin = 3;

enum i2cState
{
	idle,
	started,
	restarted
};


volatile bool clockValue;
volatile bool dataValue;
volatile bool lastClockValue {1};
volatile bool clockIsRising {0};
volatile i2cState protocolState {idle};


void setup()
{ 
	Wire.begin(I2C_ADDRESS);
	Wire.onRequest(requestEvent);
	Wire.onReceive(receiveEvent);

    // Watch the clock and data pins for changes.
	pinMode(clockPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(clockPin), onClock, CHANGE);
    pinMode(dataPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(dataPin), onData, CHANGE);

	Serial.begin(115200);
	Serial.println("Clock  Data");
}


void loop() 
{
	// Read the clock and data.
    clockValue = digitalRead(clockPin);
	dataValue = digitalRead(dataPin);
	Serial.print(clockValue * 5);
	Serial.print("   ");
	Serial.println(dataValue * 5);

	delay(40);
}


void onClock()
{
	// // Read the clock and data.
    // clockValue = digitalRead(clockPin);
	// dataValue = digitalRead(dataPin);
	// Serial.print(clockValue * 5);
	// Serial.print("   ");
	// Serial.println(dataValue * 5);

	// if (clockValue)
	// {
	// 	// Serial.print("¯");
	// 	Serial.print("/");
	// 	clockIsRising = true;
	// 	// Serial.print(dataValue ? "1" : "0");
	// }
	// else
	// {
	// 	// Serial.print("_");
	// 	Serial.print("\\");
	// 	clockIsRising = false;
	// 	// Serial.print(dataValue ? "1" : "0");
	// }

	// // volatile bool dataValue;

	// // dataValue = digitalRead(dataPin);
	// // Serial.print(dataValue ? "1" : "0");

	// // Keep track.
	// lastClockValue = clockValue;
}


void onData()
{
	// // Read the clock and data.
    // clockValue = digitalRead(clockPin);
	// dataValue = digitalRead(dataPin);
	// Serial.print(clockValue * 5);
	// Serial.print("   ");
	// Serial.println(dataValue * 5);

	// switch (protocolState)
	// {
	// 	case idle:
	// 		// Start condition.
	// 		if ((clockValue) && (!dataValue))
	// 		{
	// 			Serial.println("");
	// 			Serial.print("ST: ");
	// 			protocolState = started;
	// 		}
	// 		break;

	// 	case started:
	// 	case restarted:
	// 		Serial.println("");
	// 		// Start condition.
	// 		if ((clockValue) && (!dataValue))
	// 		{
	// 			if (protocolState == started)
	// 				Serial.print("SR: ");
	// 			else
	// 				Serial.print("RR: ");				
	// 			protocolState = restarted;
	// 		}
	// 		else if ((clockValue) && (dataValue))
	// 		{
	// 			Serial.println("SP");
	// 			protocolState = idle;
	// 		}
	// }
}


void receiveEvent(int bytes)
{
	byte opcode;

	Serial.println("receive");
	opcode = Wire.read();

	// if (bytes > 1) {
	// 	speed = Wire.read();

	// 	if (opcode == COMMAND_ON_OFF) {
	// 		speed = (speed == 0x01) ? DEFAULT_SPEED : 0;
	// 	}
	// }
}


void requestEvent()
{
	Serial.println("request");
	// switch (opcode)
	// {
	// 	case COMMAND_SPEED:
	// 		Wire.write((uint8_t*)&speed, sizeof(speed));
	// 		break;

	// 	default:
	// 		Wire.write(0);
	// 		break;
	// }
}
